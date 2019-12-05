#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "RBTree.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <cstdlib>
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

void createTestFile(std::vector<int> insertion, std::string testName) {
    DotTracer tracer(intFormatter);
    tracer.includeAddresses = false;
    tracer.drawParents = true;
    tracer.printLinkEveryStep = false;
    tracer.emitFileOnFinish = false;
    RBTree *tree = newRBTree(intCmp, intFree);
    REQUIRE(tree != NULL);
    for (auto it = insertion.begin(); it != insertion.end(); it++) {
        int value = *it;
        void *data = &*it;
        REQUIRE(addToRBTree(tree, data));
        tracer.addStep(*tree, "Added " + std::to_string(value));
    }
    std::fstream file(testName + ".dot", std::ios::out);
    std::string dot = tracer.finish("");
    file << dot;
    std::string command = "dot -Tpng " + testName+".dot" + " -o " + testName + ".png";
    std::system(command.c_str());
}


SCENARIO("Creates RB trees of proper structure", "[structure]") {
    GIVEN("10 different elements inserte in random order") {
        //std::vector<int> elements {-5,-4,-3,-2,-1,0,1,2,3,4};
        std::vector<int> elements { -3, 2, -4, 1, 3, -5, -1, 4, 0, -2};
        auto rng = std::default_random_engine {};
        // with default seed:
        // -3, 2, -4, 1, 3, -5, -1, 4, 0, -2
        //std::shuffle(elements.begin(), elements.end(), rng);

        createTestFile(elements, "test1");

        RBTree *tree = newRBTree(intCmp, intFree);
        REQUIRE(tree != NULL);

        DotTracer tracer(intFormatter);

        for (auto it = elements.begin(); it != elements.end(); it++) {
            int val = *it;
            void *data = &*it;
            REQUIRE(addToRBTree(tree, data));
            tracer.addStep(*tree, "Added " + std::to_string(val));
        }
        tracer.finish("Added 10 elements");

        THEN("It should have 10 elements") {
            REQUIRE(10 == tree->size);
            int count = 0;
            forEachRBTree(tree,[](const void* object, void* args) {
                int *count = (int*)args;
                ++*count;
                return 1;
                }, &count);
            REQUIRE(10 == count);
        }

    }
}