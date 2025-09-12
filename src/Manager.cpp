#include "Manager.h"

Manager::Manager(Sprite spruce, Sprite cherryBlossomTree, std::vector<float> positions) : gen(rd()),
m_spruce(spruce), m_cherryBlossomTree(cherryBlossomTree), m_positions(positions),
placedSpruces(-1), placedCherryBlossomTrees(-1), totalCommits(-1), totalPrs(-1), forests(-2),
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
    } else if (forests == -2) {
        std::cerr << "Failed to load forests variable from json" << '\n';
        exit(1);
    } else if (forests == -1) {
        // If someone  forks repo they will have filled background (not reset by any yaml script)
        // This method will ensure that the background is empty and the forests will be incremented
        // So after this call forests = 0 and background is empty
        resetBackground();
    }

    process();
}
void Manager::process() {
    std::cout << "processing variables\n";
    // Calculating how many of each tree needs to be added
    int spruceCount = (totalCommits / COMMITS_FOR_SPRUCE) - placedSpruces;
    int cherryBlossomTreeCount = (totalPrs / PRS_FOR_CHERRY_BLOSSOM_TREE) - placedCherryBlossomTrees;

    // Instantiating spruces
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
    // Determine path to the correct background (without any trees)
    std::string srcPath = jsonFileManager.determineBackground();
    std::string dstPath = "../assets/forest.svg";

    std::ifstream ifs(srcPath, std::ios::binary);
    if (!ifs) {
        std::cerr << "Failed to open source SVG: " << srcPath << '\n';
        return;
    }
    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();

    std::ofstream ofs(dstPath, std::ios::binary | std::ios::trunc);
    if (!ofs) {
        std::cerr << "Failed to open destination SVG for writing: " << dstPath << '\n';
        return;
    }
    ofs.write(content.data(), static_cast<std::streamsize>(content.size()));
    ofs.close();

    // Update pugixml
    background.reset();
    pugi::xml_parse_result result = background.load_string(content.c_str());
    if (!result) {
        std::cerr << "Failed to parse SVG with pugixml: " << result.description() << '\n';
        return;
    }

    background.save_file(dstPath.c_str());

    jsonFileManager.refresh();

    std::cout << "background clearing and copy successful" << '\n';
}
