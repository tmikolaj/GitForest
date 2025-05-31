#include "Manager.h"

Manager::Manager(Sprite spruce, Sprite cherryBlossomTree, std::vector<float> positions) : gen(rd()),
m_spruce(spruce), m_cherryBlossomTree(cherryBlossomTree), m_positions(positions),
placedSpruces(-1), placedCherryBlossomTrees(-1), totalCommits(-1), totalPrs(-1), forests(-1),
jsonFileManager(totalCommits, totalPrs, placedSpruces, placedCherryBlossomTrees, forests, m_positions) {

}
void Manager::init() {
    // Loading background
    background.load_file("../assets/forest.svg");

    if (!background) {
        std::cerr << "Failed to load forest.svg" << '\n';
        exit(1);
    }
    std::cout << "loaded\n";
    load();
}
void Manager::load() {
    std::cout << "loading variables from json\n";

    jsonFileManager.loadJsonFile();

    if (placedSpruces == -1) {
        std::cerr << "Failed to load placedSpruces variable from json" << '\n';
        exit(1);
    } else if (placedCherryBlossomTrees == -1) {
        std::cerr << "Failed to load placedCherryBlossomTrees variable from json\n";
        exit(1);
    } else if (totalCommits == -1) {
        std::cerr << "Failed to load totalCommits variable from json" << '\n';
        exit(1);
    } else if (totalPrs == -1) {
        std::cerr << "Failed to load totalPrs variable from json" << '\n';
        exit(1);
    } else if (forests == -1) {
        std::cerr << "Failed to load forests variable from json" << '\n';
        exit(1);
    }

    process();
}
void Manager::process() {
    std::cout << "processing variables\n";
    // Calculating how many of each tree needs to be added
    int spruceCount = (totalCommits / COMMITS_FOR_SPRUCE) - placedSpruces;
    int cherryBlossomTreeCount = (totalPrs / PRS_FOR_CHERRY_BLOSSOM_TREE) - placedCherryBlossomTrees;

    // Instantiating spruces
    // The "random" placement logic not completed!!!
    for (int i = 0; i < spruceCount; i++) {
        if (m_positions.empty()) {
            resetBackground();
        }
        std::uniform_int_distribution<> distr(0, m_positions.size() - 1);

        int index = distr(gen);
        std::cout << "spruce instantiate called\n";
        m_spruce.instantiate(m_positions[index] , TREES_Y_POS);
        m_positions.erase(m_positions.begin() + index); // erase used position

        placedSpruces++;
    }
    // Instantiating cherry blossom trees
    for (int i = 0; i < cherryBlossomTreeCount; i++) {
        if (m_positions.empty()) {
            resetBackground();
        }
        std::uniform_int_distribution<> distr(0, m_positions.size() - 1);

        int index = distr(gen);
        std::cout << "cherry blossom instantiate called\n";
        m_cherryBlossomTree.instantiate(m_positions[index] , TREES_Y_POS);
        m_positions.erase(m_positions.begin() + index); // erase used position

        placedCherryBlossomTrees++;
    }
    std::cout << "placed spruces: " << placedSpruces << '\n';
    std::cout << "placed cherry blossoms: " << placedCherryBlossomTrees << '\n';
    save();
}
void Manager::save() {
    std::cout << "saving variables\n";
    jsonFileManager.saveJsonFile();
    std::cout << "save successful\n";
}
void Manager::resetBackground() {
    background.reset();
    pugi::xml_parse_result result = background.load_file("../assets/Background.svg");

    if (!result) {
        throw std::runtime_error("Manager::resetBackground: Failed to load ../assets/Background.svg");
    }
    jsonFileManager.refresh();
}