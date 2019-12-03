//
// Created by Daniel Kerbel on 12/3/2019.
//

#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <iostream>
#include <sstream>
#include <functional>
#include <boost/intrusive/rbtree.hpp>
#include <cctype>
#include <iomanip>
#include <utility>
#include "RBTree.h"

using DataFormatter = std::function<std::string(const void*)>;

/**
 * Emits the 'dot' representation of a RB node starting from given node
 * @param node Root of tree to be drawn
 * @param label Label of node
 * @param dotStream Stream
 * @param dataFormatter Function that converts a void pointer to a string
 * @param includeAddresses Whether we should emit addresses
 */
static void treeToDot(const Node &node, const std::string &label, std::ostream &dotStream,
                      DataFormatter dataFormatter, bool includeAddresses=false)
{
    static int callCounter = 0;
    std::stringstream nodeDefinitions;
    std::stringstream edgeDefinitions;
    dotStream << "subgraph \"cluster_" << label << ++callCounter << "\"{" << std::endl;
    dotStream << "label =\"" << label << "\";" << std::endl;
    auto nodeToLabel = [&](const Node* node, const std::string &extra = "") {
        std::stringstream ss;
        ss << "\"" << label << callCounter << node << extra << "\"";
        return ss.str();
    };
    auto nodeToDef = [&](const Node* node) {
        std::stringstream ss;
        auto color = "black";
        if (node->color == RED) {
            color = "red";
        }
        if (includeAddresses) {
            ss << "[shape=record " << "color=" << color << " label=\"" << node << " | " << dataFormatter(node->data) << "\"];";
        } else {
            ss << "[shape=record " << "color=" << color  << " label=\"" << dataFormatter(node->data) << "\"];";
        }
        return ss.str();
    };
    std::function<void(const Node*)> nodeToDot = [&](const Node* node) {
        if (node == nullptr) {
            return;
        }

        nodeDefinitions << nodeToLabel(node) << nodeToDef(node) << std::endl;
        if(node->left != nullptr) {
            edgeDefinitions << nodeToLabel(node) << " -> " << nodeToLabel(node->left) << ";" << std::endl;
        } else {
            nodeDefinitions << nodeToLabel(node, "leftNull") << " [label=Null shape=point color=black];" << std::endl;
            edgeDefinitions << nodeToLabel(node) << " -> " << nodeToLabel(node, "leftNull") << ";" << std::endl;
        }
        if(node->right != nullptr) {
            edgeDefinitions << nodeToLabel(node) << " -> " << nodeToLabel(node->right) << ";" << std::endl;
        } else {
            nodeDefinitions << nodeToLabel(node, "rightNull") << " [label=Null shape=point color=black];" << std::endl;
            edgeDefinitions << nodeToLabel(node) << " -> " << nodeToLabel(node, "rightNull") << ";" << std::endl;
        }
        nodeToDot(node->left);
        nodeToDot(node->right);
    };
    nodeToDot(&node);
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


public:
    DotTracer(DataFormatter formatter = intFormatter) : m_formatter(std::move(formatter)) {
        m_stringstream << "digraph {" << std::endl;
    }

    void finish(const std::string &message="A tree") {
        m_stringstream << "label = \"" << message << "\";}" << std::endl;
        std::cout << message;
        std::cout << ", link: https://dreampuf.github.io/GraphvizOnline/#" << url_encode(m_stringstream.str()) << std::endl;
        m_stringstream.str("");
        m_stringstream.clear();
        m_stringstream << "digraph {" << std::endl;

    }

    void addStep(const Node &node, const std::string &stepName, DataFormatter *formatter = nullptr) {
        if (formatter == nullptr) {
           formatter = &m_formatter;
        }
        treeToDot(node, stepName, m_stringstream, *formatter);
    }
    void addStep(const RBTree &tree, const std::string &stepName, DataFormatter *formatter = nullptr)
    {
        if (tree.root == nullptr) {
            std::cerr << "While calling addStep, tree must have a root" << std::endl;
            exit(-1);
        }
        addStep(*tree.root, stepName, formatter);
    }

    void setFormatter(DataFormatter formatter)
    {
        m_formatter = std::move(formatter);
    }
};

#endif
