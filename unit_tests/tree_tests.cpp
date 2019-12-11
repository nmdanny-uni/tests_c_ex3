#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "RBTree.h"
#include <numeric>
#include <iostream>
#include <algorithm>
#include <random>
#include <cstdlib>
#include "tree_visualizer/util.hpp"
#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

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

        THEN("Can add 0,1,2 to it") {
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

        }

        // run tests via valgrind that there are no leaks
        freeRBTree(tree);
    }


}

std::string createTestFile(std::vector<int> insertion, std::string testName) {
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
    if (!file.is_open()) {
        std::cerr << "Error creating test file: " << std::strerror(errno) << std::endl;
    }
    std::string dot = tracer.finish("");
    file << dot;
    std::string command = "dot -Tpng " + testName+".dot" + " -o " + testName + ".png";
    std::system(command.c_str());
    freeRBTree(tree);
    return dot;
}

// paths are relative to cmake-build-debug/unit_tests folder
static std::string GENERATED_TESTS_PATH = "school-generated/";
static std::string MY_RESULTS_PATH = "impl-generated/";

static void generateTestFiles(int count) {

    auto rng = std::default_random_engine {};
    auto nodeCountGen = std::uniform_int_distribution<int>(1, 20);
    std::array<int, 50> values;
    std::iota(values.begin(), values.end(), 1);
    for (int i=0; i < count; i++) {
        auto nodeCount = nodeCountGen(rng);
        std::shuffle(values.begin(), values.end(), rng);
        std::vector<int> elements(values.begin(), values.begin() + nodeCount);
        std::stringstream fileName;
        for (auto &num: elements) {

            fileName << num << "_";
        }
        createTestFile(elements, GENERATED_TESTS_PATH + fileName.str());
    }
}

TEST_CASE("Generate test files(only to be run with school solution", "[gen]") {
#ifdef USING_SCHOOL_SOLUTION
    fs::create_directory(GENERATED_TESTS_PATH);
    generateTestFiles(15);
#endif
}

static std::vector<int> fileNameToInsertion(const std::string& fileName) {
    std::regex delimiter("_");
    std::vector<std::string> words;
    std::vector<int> nodes;
    std::copy(std::sregex_token_iterator(fileName.begin(), fileName.end(), delimiter, -1),
              std::sregex_token_iterator(),
              std::back_inserter(words));
    std::transform(words.begin(), words.end(), std::back_inserter(nodes),
        [](const std::string& word) {
       return std::stoi(word);
    });
    return nodes;
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

        freeRBTree(tree);
    }

    fs::create_directory(MY_RESULTS_PATH);
    GIVEN("Trees generated via school solution") {
        for (auto &entry: fs::directory_iterator(GENERATED_TESTS_PATH)) {
            if (entry.path().extension().string() != ".dot") {
                continue;
            }
            auto name = entry.path().filename().stem().string();

            SECTION("Testing tree at " + entry.path().filename().string())
            {
                std::ifstream expectedDotFile(entry.path());
                std::stringstream expectedDot;
                expectedDot << expectedDotFile.rdbuf();
                std::vector<int> elements = fileNameToInsertion(name);
                auto gottenDot = createTestFile(elements, MY_RESULTS_PATH + name);

                THEN("Your tree and the tree generated by the school should be identical.") {
                    REQUIRE(expectedDot.str() == gottenDot);
                }
            }
        }

    }
}
