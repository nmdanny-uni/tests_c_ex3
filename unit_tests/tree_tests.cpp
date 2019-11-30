#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "RBTree.h"
#include <iostream>


int intCmp(const void* aa, const void* bb)
{
    int *a = (int*)aa;
    int *b = (int*)bb;
    if (a < b)
    {
        return -1;
    }
    if (a == b)
    {
        return 0;
    }
    return 1;
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
            REQUIRE(addToRBTree(tree, &elements[0]));
            REQUIRE(addToRBTree(tree, &elements[1]));
            REQUIRE(addToRBTree(tree, &elements[2]));

            THEN("it contains 2, but not 3") {
                REQUIRE(containsRBTree(tree, &elements[2]));
                REQUIRE(!containsRBTree(tree, &elements[3]));
            }
            AND_WHEN("summing all elements via a foreach call") {
                int sum = 0;
                printf("calling foreach\n");
                REQUIRE(forEachRBTree(tree, foreachIntSum, &sum));

                THEN("it returns a correct sum") {
                    REQUIRE(sum == 3);
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

