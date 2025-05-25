#include "JsonFileManager.h"
#include <iostream>

JsonFileManager::JsonFileManager(int& totalCommits, int& totalPrs, int& placedSpruces, int& placedCherryBlossomTrees) : m_totalCommits(totalCommits), m_totalPrs(totalPrs), m_placedSpruces(placedSpruces), m_placedCherryBlossomTrees(placedCherryBlossomTrees) {

}
void JsonFileManager::loadJsonFile() {
    std::ifstream jsonFile("../saved.json");
    if (!jsonFile.is_open()) {
        std::cerr << "Failed to open json file\n";
        exit(1);
    }
    std::stringstream buffer;
    buffer << jsonFile.rdbuf(); // stores json data in string stream
    std::string jsonString = buffer.str(); // converts it to plain std::string
    jsonFile.close();

    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError()) {
        std::cerr << "Error parsing json file\n";
        exit(1);
    }

    if (doc.HasMember("placedSpruces") && doc["placedSpruces"].IsInt()) {
        m_placedSpruces = doc["placedSpruces"].GetInt();
    } else {
        std::cerr << "Loading json error! Missing or invalid 'placedSpruces' key in json file\n";
    }
    if (doc.HasMember("placedCherryBlossomTrees") && doc["placedCherryBlossomTrees"].IsInt()) {
        m_placedCherryBlossomTrees = doc["placedCherryBlossomTrees"].GetInt();
    } else {
        std::cerr << "Loading json error! Missing or invalid 'placedCherryBlossoms' key in json file\n";
    }
}
void JsonFileManager::saveJsonFile() {
    std::ifstream jsonFile("../saved.json");
    if (!jsonFile.is_open()) {
        std::cerr << "Failed to open json file\n";
        exit(1);
    }
    std::stringstream buffer;
    buffer << jsonFile.rdbuf();
    std::string jsonString = buffer.str();
    jsonFile.close();

    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError()) {
        std::cerr << "Error parsing json file\n";
        exit(1);
    }

    if (doc.HasMember("placedSpruces") && doc["placedSpruces"].IsInt()) {
        doc["placedSpruces"].SetInt(m_placedSpruces);
    } else {
        std::cerr << "Saving json error! Missing or invalid 'placedSpruces' key in json\n";
    }
    if (doc.HasMember("placedCherryBlossomTrees") && doc["placedCherryBlossomTrees"].IsInt()) {
        doc["placedCherryBlossomTrees"].SetInt(m_placedCherryBlossomTrees);
    }

    // Writing to json file
    rapidjson::StringBuffer outBuffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(outBuffer);
    doc.Accept(writer);

    std::ofstream outFile("../saved.json");
    if (!outFile.is_open()) {
        std::cerr << "Failed to open json file for saving\n";
        exit(1);
    }
    outFile << outBuffer.GetString();
    outFile.close();
}