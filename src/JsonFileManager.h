#ifndef JSONFILEMANAGER_H
#define JSONFILEMANAGER_H

#include "../external-libraries/rapidjson/document.h"
#include "../external-libraries/rapidjson/reader.h"
#include "../external-libraries/rapidjson/writer.h"

class JsonFileManager {
private:
    int m_totalCommits;
    int m_totalPrs;

    int m_placedSpruces;
    int m_placedCherryBlossomTrees;
public:
    JsonFileManager(int& totalCommits, int& totalPrs, int& placedSpruces, int& placedCherryBlossomTrees);
    ~JsonFileManager() = default;

    void loadJsonFile();
    void saveJsonFile();
};

#endif //JSONFILEMANAGER_H
