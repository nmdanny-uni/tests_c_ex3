//
// Created by danielkerbel on 03/12/2019.
//

#include "graph_drawer.h"

#include <utility>
#include "util.hpp"

static DotTracer TRACER = DotTracer();


void addStep(const Node *node, const char* name) {
    TRACER.addStep(*node, std::string(name));
}

void finish(const char* message) {
    TRACER.finish(std::string(message));
}

void tagNode(const Node *node, const char* tag, int steps) {
    TRACER.tagNode(node, std::string(tag), steps);
}

void setFormatter(DataFormatterC newFormatter) {
    TRACER.formatter = [=](const void* data) {
        const char* c_str = newFormatter(data);
        return std::string(c_str);
    };
}

void setIntFormatter() {
    TRACER.formatter = intFormatter;
}

void setStringFormatter() {
    TRACER.formatter = cstringFormatter;
}

void setIncludeAddresses(int includeAddresses) {
    TRACER.includeAddresses = includeAddresses;
}

void setDrawParents(int drawParents) {
    TRACER.drawParents = drawParents;
}

void setPrintLinkEveryStep(int printEveryStep) {
    TRACER.printLinkEveryStep = printEveryStep;
}

void setVisualizationEnabled(int enabled) {
    TRACER.enabled = enabled;
}
