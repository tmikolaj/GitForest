#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include "Sprite.h"

int main() {
    // Most of this code will be moved to a different file!!!
    // For now its only a test code

    // Random device and seed generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Constants for certain element to appear
    const int COMMITS_FOR_SPRUCE = 25;
    const int PRS_FOR_CHERRY_BLOSSOM_TREE = 10;

    // Keep an array of available positions
    std::vector<float> positions = { 172, 258, 344, 433, 512, 602, 682 };

    // Loading background
    pugi::xml_document background;
    background.load_file("forest.svg");
    if (!background) {
        std::cerr << "Failed to load forest.svg" << '\n';
        return 1;
    }

    Sprite spruce("spruce.svg");
    Sprite cherryBlossomTree("cherry-blossom-tree.svg");

    // Testing before using actual GitHub API
    int placedSpruces = 0;
    int placedCherryBlossomTrees = 0;

    int totalCommits = 25;
    int totalPrs = 0;

    // Calculating how many of each tree needs to be added
    // To do: refactor these 2 lines into 4
    int spruceCount = (totalCommits / COMMITS_FOR_SPRUCE) - placedSpruces;
    int cherryBlossomTreeCount = (totalPrs / PRS_FOR_CHERRY_BLOSSOM_TREE) - placedCherryBlossomTrees;

    std::cout << spruceCount << '\n';
    std::cout << cherryBlossomTreeCount << '\n';

    // Instantiating the trees
    // The "random" placement logic not completed
    for (int i = 0; i < spruceCount; i++) {
        if (positions.empty()) {
            return 0;
        }
        std::uniform_int_distribution<> distr(0, positions.size() - 1);
        spruce.instantiate(positions[distr(gen)] , 246, 100, 340);
        positions.erase(positions.begin() + distr(gen)); // erase used position
        placedSpruces++;
    }
    for (int i = 0; i < cherryBlossomTreeCount; i++) {
        if (positions.empty()) {
            return 0;
        }
        std::uniform_int_distribution<> distr(0, positions.size() - 1);
        cherryBlossomTree.instantiate(positions[distr(gen)] , 246, 115, 260);
        positions.erase(positions.begin() + distr(gen)); // erase used position
        placedCherryBlossomTrees++;
    }

    bool saveSuccessful = background.save_file("forest.svg");
    if (!saveSuccessful) {
        std::cerr << "Saving forest.svg failed!" << '\n';
    }
}