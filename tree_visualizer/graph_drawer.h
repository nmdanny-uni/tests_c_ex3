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

/// Gives the specified node a special label to appear in the next 'stepCount' steps
/// This will override any previous labels set on given node, if any exist.
/// \param node Node to be tagged
/// \param tag Label for it
/// \param stepCount Number of steps until 'finish' for the tag to appear
void tagNode(const Node *node, const char* tag, int stepCount);

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

/// Allows you to specify whether pointer addresses should be included within the visualization
/// \param includeAddresses 0 for false, anything else for true
void setIncludeAddresses(int includeAddresses);

/// Allows you to specify whether parents edges should be drawn
/// \param includeAddresses 0 for false, anything else for true
void setDrawParents(int drawParents);

/// Allows you to specify whether a link to a 'dot' graph should be printed every step, and not just when you call
/// finish()
/// \param includeAddresses 0 for false, anything else for true
void setPrintLinkEveryStep(int printEveryStep);


#ifdef __cplusplus
};
#endif

#endif //GRAPH_DRAWER_H
