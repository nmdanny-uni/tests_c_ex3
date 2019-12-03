#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "RBTree.h"
#include <iostream>
#include "tree_visualizer/util.hpp"

int intCmp(const void* aa, const void* bb)
{
    int a = *(int*)aa;
    int b = *(int*)bb;
    return a - b;
}

void intFree(void* data)
{
    // to satisfy warnings
    (void)data;
}

int foreachIntSum(const void* object, void *args)
{
    int *sum = (int*)args;
    int *obj = (int*)object;
    *sum += *obj;
    return 1;
}


TEST_CASE("Sanity check - ensure you've set up everything correctly", "[sanity check]") {
    REQUIRE(2 + 2 == 4);
}
SCENARIO("Can perform basic operations on a RB tree", "[basic]") {
    GIVEN("A newly created RB tree") {
        RBTree *tree = newRBTree(intCmp, intFree);
        REQUIRE(tree != NULL);

        WHEN("adding 0,1 and 2 to it") {
            int elements[] = { 0, 1, 2, 3};
            DotTracer tracer(intFormatter);
            REQUIRE(addToRBTree(tree, &elements[0]));
            tracer.addStep(*tree, "inserted 0");
            REQUIRE(addToRBTree(tree, &elements[1]));
            tracer.addStep(*tree, "inserted 1");
            REQUIRE(addToRBTree(tree, &elements[2]));
            tracer.addStep(*tree, "inserted 2");

            tracer.finish("Tree of 0,1,2");

            THEN("its size should be 3") {
                REQUIRE(3 == tree->size);
            }


            THEN("it contains 0,1,2, but not 3") {
                REQUIRE(containsRBTree(tree, &elements[0]));
                REQUIRE(containsRBTree(tree, &elements[1]));
                REQUIRE(containsRBTree(tree, &elements[2]));
                REQUIRE(!containsRBTree(tree, &elements[3]));
            }

            THEN("can't add a duplicate value that already exists in the tree") {
                REQUIRE(!addToRBTree(tree, &elements[2]));
                REQUIRE(tree->size == 3);
            }

            WHEN("summing all elements via a foreach call") {
                int sum = 0;
                REQUIRE(forEachRBTree(tree, foreachIntSum, &sum));

                THEN("it returns a correct sum") {
                    REQUIRE(3 == sum);
                }
            }

            AND_WHEN("freeing the tree") {
                THEN("it should free with no issues") {
                    freeRBTree(tree);
                }
            }
        }



    }


}
