#ifndef JSONFILEMANAGER_H
#define JSONFILEMANAGER_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "../external-libraries/rapidjson/document.h"
#include "../external-libraries/rapidjson/reader.h"
#include "../external-libraries/rapidjson/prettywriter.h"

class JsonFileManager {
private:
    int& m_totalCommits;
    int& m_totalPrs;

    int& m_placedSpruces;
    int& m_placedCherryBlossomTrees;

    int& m_forests;

    std::vector<float>& _m_positions;
public:
    JsonFileManager(int& totalCommits, int& totalPrs, int& placedSpruces, int& placedCherryBlossomTrees, int& forests, std::vector<float>& m_positions);
    ~JsonFileManager() = default;

    void loadJsonFile();
    void refresh();
    void saveJsonFile();
};

#endif //JSONFILEMANAGER_H
