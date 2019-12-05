//
// Created by Daniel Kerbel on 12/3/2019.
//

#ifndef UTIL_H
#define UTIL_H

#include <iterator>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <set>
#include <functional>
#include <boost/intrusive/rbtree.hpp>
#include <cctype>
#include <iomanip>
#include <utility>
#include "RBTree.h"
#include <cstdlib>
#include <fstream>
#include <cstdlib>
#include <map>

using DataFormatter = std::function<std::string(const void*)>;

/**
 * Emits the 'dot' representation of a RB node starting from given node
 * @param node Root of tree to be drawn
 * @param label Label of node
 * @param callCounter Used to differentiate between same nodes/edges that appear in multiple subgraphs
 * @param dotStream Stream
 * @param dataFormatter Function that converts a void pointer to a string
 * @param includeAddresses Whether we should emit addresses
 */
static void treeToDot(const Node &rootNode, const std::string &label, int &callCounter, std::ostream &dotStream,
                      std::map<const Node*, std::pair<std::string, int>> &tags,
                      DataFormatter dataFormatter, bool includeAddresses=false, bool drawParents=false)
{
    std::set<const Node*> visited;
    callCounter++;
    std::stringstream nodeDefinitions;
    std::stringstream edgeDefinitions;
    std::string subgraph_label = "cluster_" + label + "_count_" + std::to_string(callCounter);
    dotStream << "subgraph \"" << subgraph_label << "\"{" << std::endl;
    dotStream << "label =\"" << label << "\";" << std::endl;
    auto nodeToLabel = [&](const Node* node, const std::string &extra = "") {
        std::stringstream ss;
        ss << "\"" << subgraph_label << "_data_" << dataFormatter(node->data) << extra << "\"";
        return ss.str();
    };
    auto nodeToDef = [&](const Node* node) {
        std::stringstream ss;
        auto color = "black";
        if (node->color == RED) {
            color = "red";
        }
        std::stringstream label;
        label << "{";
        if (includeAddresses) {
            label << node << " | ";
        }
        label << dataFormatter(node->data);
        auto it = tags.find(node);
        if (it != tags.end() && it->second.second > 0) {
            label << " | " << it->second.first;
        }
        label << "}";
        ss << "[shape=record color=" << color << " label=\"" << label.str() << "\"];";
        return ss.str();
    };
    std::function<void(const Node*)> nodeToDot = [&](const Node* node) {
        if (node == nullptr) {
            return;
        }
        nodeDefinitions << nodeToLabel(node) << nodeToDef(node) << std::endl;
        if (visited.find(node) != visited.cend()) {
            std::string node_value = dataFormatter(node->data);
            fprintf(stderr, "ERROR: Infinite cycle detected while visiting %s at %p!!!!!!!!\n",
                    node_value.c_str(), node);
            return;
        }
        visited.insert(node);
        // can only draw parent if it was added to this graph(in a previous recursive call)
        if (drawParents && node->parent != nullptr && visited.find(node->parent) != visited.cend()) {
            edgeDefinitions << nodeToLabel(node) << " -> " << nodeToLabel(node->parent) << "[style=dotted];" << std::endl;
        }
        if(node->left != nullptr) {
            edgeDefinitions << nodeToLabel(node) << " -> " << nodeToLabel(node->left) << "[label=L];" << std::endl;
        } else {
            nodeDefinitions << nodeToLabel(node, "_leftNull") << " [label=Null shape=point color=black];" << std::endl;
            edgeDefinitions << nodeToLabel(node) << " -> " << nodeToLabel(node, "_leftNull") << "[label=L];" << std::endl;
        }
        if(node->right != nullptr) {
            edgeDefinitions << nodeToLabel(node) << " -> " << nodeToLabel(node->right) << "[label=R];" << std::endl;
        } else {
            nodeDefinitions << nodeToLabel(node, "_rightNull") << " [label=Null shape=point color=black];" << std::endl;
            edgeDefinitions << nodeToLabel(node) << " -> " << nodeToLabel(node, "_rightNull") << "[label=R];" << std::endl;
        }
        nodeToDot(node->left);
        nodeToDot(node->right);
    };
    nodeToDot(&rootNode);
    dotStream << nodeDefinitions.str() << std::endl;
    dotStream << edgeDefinitions.str() << std::endl;
    dotStream << "}" << std::endl;
}


// taken from https://stackoverflow.com/questions/154536/encode-decode-urls-in-c
static std::string url_encode(const std::string &value) {
    using namespace std;
    ostringstream escaped;
    escaped.fill('0');
    escaped << hex;

    for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << uppercase;
        escaped << '%' << setw(2) << int((unsigned char) c);
        escaped << nouppercase;
    }

    return escaped.str();
}


static std::string intFormatter(const void* data) {
    std::stringstream s;
    s << *(const int*)data;
    return s.str();
};


static std::string cstringFormatter(const void* data) {
    const char* cStr = *(const char**)data;
    return std::string(cStr);
}

/**
 * Class responsible for drawing each stage of a RB tree
 */
class DotTracer {
private:
    DataFormatter m_formatter;
    std::stringstream m_stringstream;
    int m_callCounter = 0;
    std::vector<std::string> m_ops;
    std::map<const Node*, std::pair<std::string, int>> m_nodeTags;

public:

    bool drawParents = true;
    bool includeAddresses = false;
    bool printLinkEveryStep = true;
    bool emitFileOnFinish = true;
    DataFormatter  formatter;

    explicit DotTracer(DataFormatter chosenFormatter = intFormatter) : formatter(std::move(chosenFormatter)) {
        m_stringstream << "digraph {" << std::endl;
    }

    std::string finish(const std::string &message="A tree")
    {
        m_stringstream << "label = \"" << message << "\";}" << std::endl;
        // for some reason, Catch captures "cout" prints but not printfs - which is why I use them here
        // and in addStep
        std::string
            msg = message + ", link: https://dreampuf.github.io/GraphvizOnline/#" + url_encode(m_stringstream.str());
        printf("%s\n", msg.c_str());

        std::string ret_val = m_stringstream.str();

        m_stringstream.str("");
        m_stringstream.clear();
        m_stringstream << "digraph {" << std::endl;
        m_callCounter = 0;

        if (emitFileOnFinish)
        {
            std::stringstream prefix;
            prefix << message + "_";
            //std::copy(ops.begin(), ops.end(), std::ostream_iterator<std::string>(prefix, "_"));
            prefix << std::to_string(m_callCounter);
            std::fstream file(prefix.str() + ".dot", std::ios::out);
            file << ret_val;
            if (file.bad()) {
                fprintf(stderr, "Couldn't write to %s.dot", prefix.str().c_str());
                exit(-1);
            }
            std::string command = "dot -Tpng \"" + prefix.str() + ".dot\"" + " -o \"" + prefix.str() + ".png\"";
            std::system(command.c_str());
        }
        m_ops.clear();
        m_nodeTags.clear();
        return ret_val;
    }

    void addStep(const Node &node, const std::string &stepName) {
        treeToDot(node, stepName, m_callCounter, m_stringstream, m_nodeTags, formatter, includeAddresses, drawParents);
        if (printLinkEveryStep)
        {
            printf(" step \"%s\" https://dreampuf.github.io/GraphvizOnline/#%s\n",
                   stepName.c_str(),
                   url_encode(m_stringstream.str() + "}").c_str());
        }
        m_ops.push_back(stepName);
        // advance/clear tags
        for (auto &tag_it: m_nodeTags) {
            --tag_it.second.second;
        }
    }
    void addStep(const RBTree &tree, const std::string &stepName)
    {
        if (tree.root == nullptr) {
            fprintf(stderr, "While calling addStep, tree must have a root\n");
            exit(-1);
        }
        addStep(*tree.root, stepName);
    }

    bool tagNode(const Node *node, const std::string& tag, int steps) {
        m_nodeTags.erase(node);
        m_nodeTags.emplace(node, std::make_pair(tag, steps));
        return true;
    }
};

#endif
