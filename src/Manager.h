#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include <random>
#include <vector>
#include "../src/Sprite.h"
#include "../src/JsonFileManager.h"
#include "pugixml.hpp"

class Manager {
private:
    // Constants for certain forest element to appear
    const int COMMITS_FOR_SPRUCE = 25;
    const int PRS_FOR_CHERRY_BLOSSOM_TREE = 10;

    const float TREES_Y_POS = 246;

    // Random device and seed generator
    std::random_device rd;
    std::mt19937 gen;

    // Keep an array of available positions
    std::vector<float> m_positions;

    pugi::xml_document background;

    Sprite m_spruce;
    Sprite m_cherryBlossomTree;

    int placedSpruces;
    int placedCherryBlossomTrees;

    int totalCommits;
    int totalPrs;

    int forests;

    JsonFileManager jsonFileManager;

    void load(); // loads all stored data
    void process(); // checks how many trees to place and places them
    void save(); //saves all data (overwriting previously saved one
    void resetBackground(); // resets background (clears xml code) then loads fresh background and calls refresh method
    // from JsonFileManager class which refills vector with available positions for trees
public:
    Manager(Sprite spruce, Sprite cherryBlossomTree, std::vector<float> positions);
    ~Manager() = default;

    void init();
};

#endif //MANAGER_H
