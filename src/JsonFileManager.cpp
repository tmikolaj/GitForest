#include "JsonFileManager.h"
#include <iostream>

JsonFileManager::JsonFileManager(int& totalCommits, int& totalPrs, int& placedSpruces, int& placedCherryBlossomTrees, int& forests, std::vector<float>& m_positions) :
m_totalCommits(totalCommits),
m_totalPrs(totalPrs),
m_placedSpruces(placedSpruces),
m_placedCherryBlossomTrees(placedCherryBlossomTrees),
m_forests(forests),
_m_positions(m_positions) {

}
void JsonFileManager::loadJsonFile() {
    std::ifstream ifs("../saved.json");
    if (!ifs.is_open()) {
        throw std::runtime_error("JsonFileManager::loadJsonFile: Could not open json file ('../saved.json')");
    }

    std::stringstream buffer;
    buffer << ifs.rdbuf(); // stores json data in string stream
    std::string jsonString = buffer.str(); // converts it to plain std::string
    ifs.close();

    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError()) {
        throw std::runtime_error("JsonFileManager::loadJsonFile: Failed to parse json file ('../saved.json')");
    }

    if (doc.HasMember("placedSpruces") && doc["placedSpruces"].IsInt()) {
        m_placedSpruces = doc["placedSpruces"].GetInt();
    } else {
        throw std::runtime_error("JsonFileManager::loadJsonFile: missing or invalid 'placedSpruces'");
    }

    if (doc.HasMember("placedCherryBlossomTrees") && doc["placedCherryBlossomTrees"].IsInt()) {
        m_placedCherryBlossomTrees = doc["placedCherryBlossomTrees"].GetInt();
    } else {
        throw std::runtime_error("JsonFileManager::loadJsonFile: Missing or invalid 'placedCherryBlossomTrees'");
    }

    if (doc.HasMember("availablePos")) {

        const rapidjson::Value& _availablePos = doc["availablePos"];

        for (rapidjson::SizeType i = 0; i < _availablePos.Size(); i++) {
            if (_availablePos[i].IsNumber()) {
                _m_positions.push_back(_availablePos[i].GetFloat());
            }
        }
    }

    if (doc.HasMember("forests") && doc["forests"].IsInt()) {
        m_forests = doc["forests"].GetInt();
    } else {
        throw std::runtime_error("JsonFileManager::loadJsonFile: Missing or invalid 'forests'");
    }

    std::ifstream ifsApiData("../apidata.json");
    if (!ifsApiData.is_open()) {
        throw std::runtime_error("JsonFileManager::loadJsonFile: Could not open json file ('../apidata.json')");
    }

    std::stringstream bufferApiData;
    bufferApiData << ifsApiData.rdbuf();
    std::string apiData = bufferApiData.str();
    ifsApiData.close();

    rapidjson::Document apidoc;
    apidoc.Parse(apiData.c_str());

    if (apidoc.HasParseError()) {
        throw std::runtime_error("JsonFileManager::loadJsonFile: Failed to parse json file ('../apidata.json')");
    }

    if (apidoc.HasMember("commits") && apidoc["commits"].IsInt()) {
        m_totalCommits = apidoc["commits"].GetInt();
    } else {
        throw std::runtime_error("JsonFileManager::loadJsonFile: Missing or invalid 'commits'");
    }

    if (apidoc.HasMember("prs") && apidoc["prs"].IsInt()) {
        m_totalPrs = apidoc["prs"].GetInt();
    } else {
        throw std::runtime_error("JsonFileManager::loadJsonFile: Missing or invalid 'prs'");
    }
}
void JsonFileManager::refresh() {
    if (!_m_positions.empty()) {
        return;
    }

    std::ifstream ifs("../saved.json");
    if (!ifs.is_open()) {
        throw std::runtime_error("JsonFileManager::refresh: Could not open json file ('../saved.json')");
    }

    std::stringstream buffer;
    buffer << ifs.rdbuf();
    std::string jsonString = buffer.str();
    ifs.close();

    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError()) {
        throw std::runtime_error("JsonFileManager::refresh: Failed to parse json file");
    }

    if (doc.HasMember("positions") && doc["positions"].IsArray()) {

        const rapidjson::Value& _positions = doc["positions"];

        for (rapidjson::SizeType i = 0; i < _positions.Size(); i++) {
            if (_positions[i].IsNumber()) {
                _m_positions.push_back(_positions[i].GetFloat());
            }
        }
    } else {
        throw std::runtime_error("JsonFileManager::refresh: Missing or invalid 'positions'");
    }

    m_forests++;
}
void JsonFileManager::saveJsonFile() {
    std::ifstream ifs("../saved.json");
    if (!ifs.is_open()) {
        throw std::runtime_error("JsonFileManager::saveJsonFile: Could not open json file ('../saved.json')");
    }

    std::stringstream buffer;
    buffer << ifs.rdbuf();
    std::string jsonString = buffer.str();
    ifs.close();

    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError()) {
        throw std::runtime_error("JsonFileManager::saveJsonFile: Failed to parse json file ('../saved.json')");
    }

    if (doc.HasMember("placedSpruces") && doc["placedSpruces"].IsInt()) {
        doc["placedSpruces"].SetInt(m_placedSpruces);
    } else {
        throw std::runtime_error("JsonFileManager::saveJsonFile: Missing or invalid 'placedSpruces'");
    }

    if (doc.HasMember("placedCherryBlossomTrees") && doc["placedCherryBlossomTrees"].IsInt()) {
        doc["placedCherryBlossomTrees"].SetInt(m_placedCherryBlossomTrees);
    } else {
        throw std::runtime_error("JsonFileManager::saveJsonFile: Missing or invalid 'placedCherryBlossomTrees'");
    }

    if (doc.HasMember("availablePos")) {
        rapidjson::Value& posArray = doc["availablePos"];
        posArray.SetArray(); // Emptying array

        rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

        for (float pos : _m_positions) {
            posArray.PushBack(static_cast<int>(pos), allocator);
        }
    } else {
        throw std::runtime_error("JsonFileManager::saveJsonFile: Missing or invalid 'availablePos'");
    }

    if (doc.HasMember("forests") && doc["forests"].IsInt()) {
        doc["forests"].SetInt(m_forests);
    } else {
        throw std::runtime_error("JsonFileManager::saveJsonFile: Missing or invalid 'forests'");
    }

    // Writing to json file
    std::ofstream ofs("../saved.json");
    if (!ofs.is_open()) {
        throw std::runtime_error("JsonFileManager::saveJsonFile: Could not open json file ('../saved.json')");
    }

    rapidjson::StringBuffer outBuffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(outBuffer);
    doc.Accept(writer);

    ofs << outBuffer.GetString();
    ofs.close();
}
std::string JsonFileManager::determineBackground() {
    std::ifstream ifs("../config.json");
    if (!ifs.is_open()) {
        throw std::runtime_error("JsonFileManager::determineBackground: Could not open json file '../config.json'");
    }

    std::stringstream buffer;
    buffer << ifs.rdbuf();
    std::string jsonString = buffer.str();
    ifs.close();

    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError()) {
        throw std::runtime_error("JsonFileManager::determineBackground: Failed to parse json file '../config.json'");
    }

    std::string colorsAllowed[8] = { "mystic", "golden", "frost", "emerald", "purple", "yellow", "blue", "green" };
    std::string toReturn[8] = { "background-purple", "background-yellow", "background-blue", "background-green", "background-purple", "background-yellow", "background-blue", "background-green" };

    if (doc.HasMember("backgroundColor") && doc["backgroundColor"].IsString()) {
        for (int i = 0; i < 8; i++) {
            if (colorsAllowed[i] == doc["backgroundColor"].GetString()) {
                if (doc["backgroundColor"].GetString() != doc["currColor"].GetString()) {
                   doc["currColor"].SetString(doc["backgroundColor"].GetString(), doc.GetAllocator());

                    rapidjson::StringBuffer jsonbuffer;
                    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(jsonbuffer);
                    doc.Accept(writer);

                    std::ofstream ofs("../config.json");
                    if (!ofs) {
                        throw std::runtime_error("JsonFileManager::determineBackground: Could not open json file '../config.json'");
                    }
                    ofs << jsonbuffer.GetString();
                    ofs.close();
                }
                return "../assets/" + toReturn[i] + ".svg";
            }
            throw std::runtime_error("JsonFileManager::determineBackground: Invalid color in config");
        }
    } else {
        throw std::runtime_error("JsonFileManager::determineBackground: Missing or invalid 'backgroundColor'");
    }

    return "";
}