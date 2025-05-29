#include "src/Manager.h"

int main() {
    std::vector<float> positions;
    Sprite spruce("../assets/spruce.svg");
    Sprite cherryBlossomTree("../assets/cherry-blossom-tree.svg");

    Manager manager(spruce, cherryBlossomTree, positions);
    manager.init();
}