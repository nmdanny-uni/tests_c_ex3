//
// Created by danielkerbel on 05/12/2019.
//

#include "RBTree.h"
#include "Structs.h"
#include "catch.hpp"
#include <iterator>
#include <set>
#include <algorithm>
#include <cstring>
#include "tree_visualizer/util.hpp"

static Vector* args_to_vector(std::initializer_list<double> args)
{
    double *buff = (double*)malloc(sizeof(double) * args.size());
    auto *vec = (Vector*)malloc(sizeof(Vector));
    vec->len = args.size();
    vec->vector = buff;
    std::copy(args.begin(), args.end(), buff);
    return vec;
}

static RBTree* vectors_to_tree(std::initializer_list<Vector*> vectors) {
    RBTree* tree = newRBTree(vectorCompare1By1, freeVector);
    for (auto vector: vectors)
    {
        addToRBTree(tree, vector);
    }
    return tree;
}


static RBTree* strings_to_tree(const std::initializer_list<std::string> &strings) {
    RBTree* tree = newRBTree(stringCompare, freeString);
    for (const auto &string: strings)
    {
        const char* c_string = string.c_str();
        char* c_string_dynamic = strdup(c_string);

        addToRBTree(tree, (void*)c_string_dynamic);
    }
    return tree;
}

SCENARIO("String tree operations", "[tree]")
{
    GIVEN("A RB tree with some dynamically allocated strings")
    {
        std::initializer_list<std::string> strings = {
            "hey", "test"
        };
        // sanity check - ensure we didn't accidentally place duplicates in the above list
        REQUIRE(std::set<std::string>(strings).size() == strings.size());

        RBTree *tree = strings_to_tree(strings);
        REQUIRE(tree->size == (int)strings.size());

        std::vector<std::string> sortedStrings(strings);
        std::sort(sortedStrings.begin(), sortedStrings.end());


        WHEN("forEachRBTree should iterate it in ascending order(tree-inorder)")
        {
            std::vector<std::string> gotten;
            forEachRBTree(tree, [](const void *data, void *args) {
                const char *string = (const char*)data;
                auto outVec = (decltype(&gotten))args;
                outVec->push_back(string);
                return 1;
                }, (void*)&gotten);
            THEN("It should produce a sorted list of strings, joined with newlines") {
                REQUIRE_THAT(sortedStrings, Catch::Matchers::Equals(gotten));
            }
        }
        WHEN("using 'concatenate' as a forEach function") {
            std::stringstream expected;
            std::copy(sortedStrings.begin(), sortedStrings.end(), std::ostream_iterator<std::string>(expected, "\n"));
            char *buf = (char*)calloc(expected.str().size() + 1,sizeof(char));
            forEachRBTree(tree, concatenate, buf);
            std::string gotten = std::string(buf);
            REQUIRE(expected.str() == gotten);
            free(buf);
        }
        freeRBTree(tree);
    }

}


SCENARIO("Vector operations", "[vector]")
{
    GIVEN("4 vectors of same length o=[0,0,0], oDup=[0,0,0], u=[1,2,0] and v=[0,2,1]")
    {
        Vector *o = args_to_vector({0, 0, 0});
        Vector *u = args_to_vector({1, 2, 0});
        Vector *v = args_to_vector({0, 2, 1});
        Vector *oDup = args_to_vector({0, 0, 0});

        WHEN("Doing elementwise comparison of various vectors")
        {
            THEN("o<u")
            {
                int cmp = vectorCompare1By1(o, u);
                REQUIRE(cmp < 0);
            }THEN("u>o")
            {
                int cmp = vectorCompare1By1(u, o);
                REQUIRE(cmp > 0);
            }THEN("u=u, o=o=oDup, v=v")
            {
                CHECK(vectorCompare1By1(u, u) == 0);
                CHECK(vectorCompare1By1(o, o) == 0);
                CHECK(vectorCompare1By1(v, v) == 0);
                CHECK((vectorCompare1By1(o, oDup) == 0 && vectorCompare1By1(oDup, o) == 0));
            }
        }

        // run tests via valgrind to enforce there's no memory leak
        freeVector(o);
        freeVector(u);
        freeVector(v);
        freeVector(oDup);

    }

    GIVEN("Some vectors in a RB tree") {
        auto expectedMaxVector = args_to_vector({0,4,4,200});
        auto vectors = {
            args_to_vector({7,3,3,1}),
            args_to_vector({1,5,5,7}),
            expectedMaxVector,
            args_to_vector({20,0,0, 1}),
            args_to_vector({0,0,0,0}),
            args_to_vector({200, 0, 0, 0})};
        RBTree* tree = vectors_to_tree(vectors);

        THEN("You can find the vector with the largest norm correctly") {
            Vector* maxVector = findMaxNormVectorInTree(tree);
            REQUIRE(maxVector != NULL);
            double* maxBuff = maxVector->vector;
            double* expectedMaxBuff = expectedMaxVector->vector;
            CHECK(4 == maxVector->len);
            CHECK(std::equal(maxBuff, maxBuff+4, expectedMaxBuff, expectedMaxBuff+4));
            freeVector(maxVector);
        }

        freeRBTree(tree);
    }
}