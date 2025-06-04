#include "src/Manager.h"

std::string determinePath();

int main() {
    std::vector<float> positions;
    Sprite spruce(determinePath());
    Sprite cherryBlossomTree("../assets/cherry-blossom-tree.svg");

    Manager manager(spruce, cherryBlossomTree, positions);
    manager.init();
}
std::string determinePath() {
    std::ifstream ifs("../config.json");
    if (!ifs.is_open()) {
        throw std::runtime_error("JsonFileManager::determinePath: Could not open json file ('../config.json')");
    }

    std::stringstream buffer;
    buffer << ifs.rdbuf();
    std::string jsonString = buffer.str();
    ifs.close();

    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError()) {
        throw std::runtime_error("JsonFileManager::determinePath: Failed to parse json file");
    }

    std::string colorsAllowed[8] = { "mystic", "golden", "frost", "emerald", "purple", "yellow", "blue", "green" };
    std::string toReturn[8] = { "spruce-purple", "spruce-yellow", "spruce-blue", "spruce-green", "spruce-purple", "spruce-yellow", "spruce-blue", "spruce-green" };

    if (doc.HasMember("currColor") && doc["currColor"].IsString()) {
        for (int i = 0; i < 8; i++) {
            if (colorsAllowed[i] == doc["currColor"].GetString()) {
                return "../assets/" + toReturn[i] + ".svg";
            }
        }
        throw std::runtime_error("JsonFileManager::determinePath: Invalid color set in config");
    } else {
        throw std::runtime_error("JsonFileManager::determinePath: Missing or invalid 'currColor'");
    }
    return "";
}