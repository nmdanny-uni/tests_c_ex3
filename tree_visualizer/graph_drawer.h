//
// Created by danielkerbel on 03/12/2019.
//


#ifndef GRAPH_DRAWER_H
#define GRAPH_DRAWER_H
#include "RBTree.h"

#ifdef __cplusplus
extern "C" {
#endif


/// The type of a function that takes a node's data(void pointer) and returns a C string for it
typedef char*(*DataFormatterC)(const void*);

/// Creates a snapshot of the tree's current state in the graph into the global dot emitter
/// \param node Node representing root of the tree - usually an RBTree's root, but can also be a deeper node if you wish
/// \param name A label for this snapshot, usually what kind of step was done
void addStep(const Node *node, const char* name);

/// Creates an entire .dot graph comprising of several subgraphs for each step that was added
///
/// \param message A label for the entire graph
void finish(const char* message);

/// Uses the included int formatter
void setIntFormatter();

/// Uses the included string formatter
void setStringFormatter();

/// Allows you to specify your own formatter for a certain datatype.
void setFormatter(DataFormatterC newFormatter);


#ifdef __cplusplus
};
#endif

#endif //GRAPH_DRAWER_H
