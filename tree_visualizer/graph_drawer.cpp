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

void setFormatter(DataFormatterC newFormatter) {
    DataFormatter properFmter = [&](const void* data) {
        return std::string(newFormatter(data));
    };
    TRACER.setFormatter(properFmter);
}

void setFormatter(DataFormatter newFormatter) {
    TRACER.setFormatter(std::move(newFormatter));
}

void setIntFormatter() {
    TRACER.setFormatter(intFormatter);
}

void setStringFormatter() {
    TRACER.setFormatter(cstringFormatter);
}
