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
    DataFormatter properFmter = [&](const void* data) {
        return std::string(newFormatter(data));
    };
    TRACER.formatter = properFmter;
}

void setFormatter(DataFormatter newFormatter) {
    TRACER.formatter = std::move(newFormatter);
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

