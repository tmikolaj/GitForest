#include "JsonFileManager.h"
#include <iostream>

JsonFileManager::JsonFileManager(int& totalCommits, int& totalPrs, int& placedSpruces, int& placedCherryBlossomTrees) : m_totalCommits(totalCommits), m_totalPrs(totalPrs), m_placedSpruces(placedSpruces), m_placedCherryBlossomTrees(placedCherryBlossomTrees) {

}
void JsonFileManager::loadJsonFile() {
    std::ifstream jsonFile("../saved.json");
    if (!jsonFile.is_open()) {
        throw std::runtime_error("JsonFileManager::loadJsonFile: Could not open json file ('../saved.json')");
    }
    std::stringstream buffer;
    buffer << jsonFile.rdbuf(); // stores json data in string stream
    std::string jsonString = buffer.str(); // converts it to plain std::string
    jsonFile.close();

    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError()) {
        throw std::runtime_error("JsonFileManager::loadJsonFile(): Failed to parse json file ('../saved.json')");
    }

    if (doc.HasMember("placedSpruces") && doc["placedSpruces"].IsInt()) {
        m_placedSpruces = doc["placedSpruces"].GetInt();
    } else {
        throw std::runtime_error("JsonFileManager::loadJsonFile(): missing or invalid 'placedSpruces'");
    }
    if (doc.HasMember("placedCherryBlossomTrees") && doc["placedCherryBlossomTrees"].IsInt()) {
        m_placedCherryBlossomTrees = doc["placedCherryBlossomTrees"].GetInt();
    } else {
        throw std::runtime_error("JsonFileManager::loadJsonFile(): missing or invalid 'placedCherryBlossomTrees'");
    }
}
void JsonFileManager::saveJsonFile() {
    std::ifstream jsonFile("../saved.json");
    if (!jsonFile.is_open()) {
        throw std::runtime_error("JsonFileManager::saveJsonFile: Could not open json file ('../saved.json')");
    }
    std::stringstream buffer;
    buffer << jsonFile.rdbuf();
    std::string jsonString = buffer.str();
    jsonFile.close();

    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError()) {
        throw std::runtime_error("JsonFileManager::loadJsonFile(): Failed to parse json file ('../saved.json')");
    }

    if (doc.HasMember("placedSpruces") && doc["placedSpruces"].IsInt()) {
        doc["placedSpruces"].SetInt(m_placedSpruces);
    } else {
        throw std::runtime_error("JsonFileManager::loadJsonFile(): missing or invalid 'placedSpruces'");
    }
    if (doc.HasMember("placedCherryBlossomTrees") && doc["placedCherryBlossomTrees"].IsInt()) {
        doc["placedCherryBlossomTrees"].SetInt(m_placedCherryBlossomTrees);
    } else {
        throw std::runtime_error("JsonFileManager::loadJsonFile(): missing or invalid 'placedCherryBlossomTrees'");
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