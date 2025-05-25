#include "src/Manager.h"

int main() {
    // The current code is written only for testing purposes
    std::vector<float> positions = { 172, 258, 344, 433, 512, 602, 682 };
    Sprite spruce("../spruce.svg");
    Sprite cherryBlossomTree("../cherry-blossom-tree.svg");

    Manager manager(spruce, cherryBlossomTree, positions);
    manager.init();
}