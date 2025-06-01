#include <vector>
#include <iostream>
#include "catch_amalgamated.hpp"
#include "globals.h"

const int COMMITS_FOR_SPRUCE = 25;
const int PRS_FOR_CHERRY_BLOSSOM_TREE = 10;

int resSpruce;
int resCherryBlossomTree;

std::vector<float> m_positions = { 172, 258, 344, 433, 512, 602, 682 };

void process() {
    if (totalCommits / COMMITS_FOR_SPRUCE < placedSpruces) {
        std::cerr << "Invalid value for variable totalCommits!\n";
        throw std::logic_error("Invalid value for variable totalCommits");
    }
    // Calculating how many of each tree needs to be added
    int spruceCount = (totalCommits / COMMITS_FOR_SPRUCE) - placedSpruces;
    int cherryBlossomTreeCount = (totalPrs / PRS_FOR_CHERRY_BLOSSOM_TREE) - placedCherryBlossomTrees;

    resSpruce = spruceCount;
    resCherryBlossomTree = cherryBlossomTreeCount;
    // Instantiating spruces
    // The "random" placement logic not completed!!!
    for (int i = 0; i < spruceCount; i++) {
        placedSpruces++;
    }
    // Instantiating cherry blossom trees
    for (int i = 0; i < cherryBlossomTreeCount; i++) {
        placedCherryBlossomTrees++;
    }
}
void reset() {
    resSpruce = 0;
    resCherryBlossomTree = 0;
}

TEST_CASE("Spruce placement logic", "[trees]") {
    SECTION("No commits, no spruces") {
        reset();
        totalCommits = 0;
        placedSpruces = 0;
        process();
        REQUIRE(resSpruce == 0);
    }

    SECTION("Exactly one spruce needed") {
        reset();
        totalCommits = 25;
        placedSpruces = 0;
        process();
        REQUIRE(resSpruce == 1);
    }

    SECTION("Already placed all needed spruces") {
        reset();
        totalCommits = 75;
        placedSpruces = 3;
        process();
        REQUIRE(resSpruce == 0);
    }

    SECTION("Some spruces placed, more needed") {
        reset();
        totalCommits = 100;
        placedSpruces = 2;
        process();
        REQUIRE(resSpruce == 2);
    }

    SECTION("Negative result doesn't happen") {
        reset();
        totalCommits = 25;
        placedSpruces = 2;
        REQUIRE_THROWS_AS(process(), std::logic_error); // Updated: negative values should be prevented by logic
    }

    SECTION("Random input") {
        reset();
        totalCommits = 498;
        placedSpruces = 10;
        process();
        REQUIRE(resSpruce == 9);
    }
}

TEST_CASE("Cherry blossom placement logic", "[trees]") {
    SECTION("Exactly one cherry blossom needed") {
        reset();
        totalPrs = 10;
        placedCherryBlossomTrees = 0;
        process();
        REQUIRE(resCherryBlossomTree == 1);
    }

    SECTION("Already placed all needed cherry blossoms") {
        reset();
        totalPrs = 50;
        placedCherryBlossomTrees = 5;
        process();
        REQUIRE(resCherryBlossomTree == 0);
    }

    SECTION("Some cherry blossoms placed, more needed") {
        reset();
        totalPrs = 80;
        placedCherryBlossomTrees = 6;
        process();
        REQUIRE(resCherryBlossomTree == 2);
    }
}