//
// Created by danielkerbel on 03/12/2019.
//

#include "graph_drawer.h"

#include <utility>
#include "util.hpp"

static DotTracer TRACER = DotTracer();

#ifdef ENABLE_TREE_VISUALIZATION
#define TRACER_OP(STMT) (STMT)
#else
#define TRACER_OP(STMT)
#endif

void addStep(const Node *node, const char* name) {
    TRACER_OP(TRACER.addStep(*node, std::string(name)));
}

void finish(const char* message) {
    TRACER_OP(TRACER.finish(std::string(message)));
}

void tagNode(const Node *node, const char* tag, int steps) {
    TRACER_OP(TRACER.tagNode(node, std::string(tag), steps));
}

void setFormatter(DataFormatterC newFormatter) {
   TRACER_OP(TRACER.formatter = [=](const void* data) {
        const char* c_str = newFormatter(data);
        return std::string(c_str);
    });
}

void setIntFormatter() {
    TRACER_OP(TRACER.formatter = intFormatter);
}

void setStringFormatter() {
    TRACER_OP(TRACER.formatter = cstringFormatter);
}

void setIncludeAddresses(int includeAddresses) {
    TRACER_OP(TRACER.includeAddresses = includeAddresses);
}

void setDrawParents(int drawParents) {
    TRACER_OP(TRACER.drawParents = drawParents);
}

void setPrintLinkEveryStep(int printEveryStep) {
    TRACER_OP(TRACER.printLinkEveryStep = printEveryStep);
}

void setVisualizationEnabled(int enabled) {
    TRACER_OP(TRACER.enabled = enabled);
}
