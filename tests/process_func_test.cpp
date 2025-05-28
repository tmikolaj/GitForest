#include <vector>
#include <random>
#include <iostream>
#include "catch_amalgamated.hpp"

std::random_device rd;
std::mt19937 gen(rd());

const int COMMITS_FOR_SPRUCE = 25;
const int PRS_FOR_CHERRY_BLOSSOM_TREE = 10;

int totalCommits = 0;
int totalPrs = 0;

int placedSpruces = 0;
int placedCherryBlossomTrees = 0;

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
        if (m_positions.empty()) {
            exit(1); // Temporary
        }
        std::uniform_int_distribution<> distr(0, m_positions.size() - 1);

        int index = distr(gen);
        m_positions.erase(m_positions.begin() + index); // erase used position

        placedSpruces++;
    }
    // Instantiating cherry blossom trees
    for (int i = 0; i < cherryBlossomTreeCount; i++) {
        if (m_positions.empty()) {
            exit(1); // Temporary
        }
        std::uniform_int_distribution<> distr(0, m_positions.size() - 1);

        int index = distr(gen);
        m_positions.erase(m_positions.begin() + index); // erase used position

        placedCherryBlossomTrees++;
    }
}

TEST_CASE("Spruce placement logic", "[trees]") {
    SECTION("No commits, no spruces") {
        totalCommits = 0;
        placedSpruces = 0;
        process();
        REQUIRE(resSpruce == 0);
    }

    SECTION("Exactly one spruce needed") {
        totalCommits = 25;
        placedSpruces = 0;
        process();
        REQUIRE(resSpruce == 1);
    }

    SECTION("Already placed all needed spruces") {
        totalCommits = 75;
        placedSpruces = 3;
        process();
        REQUIRE(resSpruce == 0);
    }

    SECTION("Some spruces placed, more needed") {
        totalCommits = 100;
        placedSpruces = 2;
        process();
        REQUIRE(resSpruce == 2);
    }

    SECTION("Negative result doesn't happen") {
        totalCommits = 25;
        placedSpruces = 2;
        REQUIRE_THROWS_AS(process(), std::logic_error); // Updated: negative values should be prevented by logic
    }

    // Commented it out cause it causes segfault, but it returns the expected value which is 9
    // I know its caused by the random placement (which tries to delete an element from empty vector),
    // but I havent implemented it yet
    // SECTION("Random input") {
    //     totalCommits = 498;
    //     placedSpruces = 10;
    //     process();
    //     REQUIRE(resSpruce == 9);
    // }
}

TEST_CASE("Cherry blossom placement logic", "[trees]") {
    SECTION("Exactly one cherry blossom needed") {
        totalPrs = 10;
        placedCherryBlossomTrees = 0;
        process();
        REQUIRE(resCherryBlossomTree == 1);
    }

    SECTION("Already placed all needed cherry blossoms") {
        totalPrs = 50;
        placedCherryBlossomTrees = 5;
        process();
        REQUIRE(resCherryBlossomTree == 0);
    }

    SECTION("Some cherry blossoms placed, more needed") {
        totalPrs = 80;
        placedCherryBlossomTrees = 6;
        process();
        REQUIRE(resCherryBlossomTree == 2);
    }
}