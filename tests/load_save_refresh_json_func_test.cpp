#include <fstream>
#include <cstdio> // std::remove (for file removing)
#include <string>
#include "../external-libraries/rapidjson/document.h"
#include "../external-libraries/rapidjson/prettywriter.h"
#include "../external-libraries/rapidjson/stringbuffer.h"
#include "../src/JsonFileManager.h"
#include "catch_amalgamated.hpp"
#include "globals.h"

static void write_json_file(int spruce, int cherry, int forests, std::vector<float> positions) {
    rapidjson::Document doc;
    doc.SetObject();
    auto& alloc = doc.GetAllocator();

    doc.AddMember("placedSpruces", spruce, alloc);
    doc.AddMember("placedCherryBlossomTrees", cherry, alloc);

    std::vector<float> _positions = {172, 258, 344, 433, 512, 602, 682};
    rapidjson::Value positionsArray(rapidjson::kArrayType);
    for (int i = 0; i < _positions.size(); i++) {
        positionsArray.PushBack(_positions[i], alloc);
    }
    doc.AddMember("positions", positionsArray, alloc);

    rapidjson::Value availablePosArray(rapidjson::kArrayType);
    for (int i = 0; i < positions.size(); i++) {
        availablePosArray.PushBack(positions[i], alloc);
    }
    doc.AddMember("availablePos", availablePosArray, alloc);

    doc.AddMember("forests", forests, alloc);

    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);

    std::ofstream ofs("../saved.json");
    REQUIRE(ofs.is_open());
    ofs << sb.GetString();
    ofs.close();
}
static rapidjson::Document read_json_file() {
    std::ifstream ifs("../saved.json");
    REQUIRE(ifs.is_open());
    std::string content{std::istreambuf_iterator<char>(ifs), {}};
    rapidjson::Document doc;
    doc.Parse(content.c_str());
    REQUIRE_FALSE(doc.HasParseError());
    return doc;
}
static void remove_saved_json() {
    std::remove("../saved.json");
}
static bool checkVector(const std::vector<float>& v, const std::vector<float>& expected) {
    if (v.size() != expected.size()) return false;

    int n = expected.size();

    for (int i = 0; i < n; i++) {
        if (v[i] != expected[i]) {
            return false;
        }
    }
    return true;
}
static void write_apidata_file(int commits, int prs) {
    rapidjson::Document doc;
    doc.SetObject();
    auto& alloc = doc.GetAllocator();

    doc.AddMember("commits", commits, alloc);
    doc.AddMember("prs", prs, alloc);

    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    doc.Accept(writer);

    std::ofstream ofs("../apidata.json");
    REQUIRE(ofs.is_open());
    ofs << sb.GetString();
    ofs.close();
}
static void remove_apidata_file() {
    std::remove("../apidata.json");
}
TEST_CASE("loadJsonFile() success path", "[load]") {
    remove_saved_json();
    remove_apidata_file();

    std::vector<float> expected = {172, 258, 344, 433, 512, 602, 682};

    write_apidata_file(75, 50);
    write_json_file(3, 5, 1, expected);

    totalCommits = 0;
    totalPrs = 0;
    placedSpruces = -1;
    placedCherryBlossomTrees = -1;
    std::vector<float> positions = {};
    forests = 0;

    JsonFileManager jsonFileManager(totalCommits, totalPrs, placedSpruces, placedCherryBlossomTrees, forests, positions);

    REQUIRE_NOTHROW(jsonFileManager.loadJsonFile());

    REQUIRE(totalCommits == 75);
    REQUIRE(totalPrs == 50);

    REQUIRE(placedSpruces == 3);
    REQUIRE(placedCherryBlossomTrees == 5);
    REQUIRE(forests == 1);

    REQUIRE(checkVector(positions, expected) == true);

    remove_saved_json();
    remove_apidata_file();
}
TEST_CASE("saveJsonFile() overwrites correctly", "[save]") {
    remove_saved_json();
    remove_apidata_file();

    write_apidata_file(0, 0);
    write_json_file(0, 0, 0, {});


    totalCommits = 0;
    totalPrs = 0;
    placedSpruces = 7;
    placedCherryBlossomTrees = 9;
    std::vector<float> positions = {172, 433, 512};
    forests = 2;

    JsonFileManager jsonFileManager(totalCommits, totalPrs, placedSpruces, placedCherryBlossomTrees, forests, positions);

    REQUIRE_NOTHROW(jsonFileManager.saveJsonFile());

    rapidjson::Document doc = read_json_file();

    REQUIRE(doc["placedSpruces"].IsInt());
    REQUIRE(doc["placedCherryBlossomTrees"].IsInt());
    REQUIRE(doc["availablePos"].IsArray());
    REQUIRE(doc["positions"].IsArray());
    REQUIRE(doc["forests"].IsInt());

    REQUIRE(doc["placedSpruces"].GetInt() == 7);
    REQUIRE(doc["placedCherryBlossomTrees"].GetInt() == 9);

    std::vector<float> expected = {172, 258, 344, 433, 512, 602, 682};
    const rapidjson::Value& posArr = doc["positions"];
    for (auto i = 0; i < posArr.Size(); i++) {
        REQUIRE(posArr[i] == expected[i]);
    }

    expected = {172, 433, 512};
    const rapidjson::Value& avPosArr = doc["availablePos"];
    for (auto i = 0; i < avPosArr.Size(); i++) {
        REQUIRE(avPosArr[i] == expected[i]);
    }

    REQUIRE(doc["forests"] == 2);

    remove_saved_json();
    remove_apidata_file();
}
TEST_CASE("loadJsonFile() throws on missing file", "[load][error]") {
    remove_saved_json();
    remove_apidata_file();

    int tc=0, tp=0, ps=0, pc=0, fr=0;
    std::vector<float> psv = {};
    JsonFileManager mgr(tc,tp,ps,pc,fr,psv);
    REQUIRE_THROWS_AS(mgr.loadJsonFile(), std::runtime_error);
}
TEST_CASE("loadJsonFile() throws on bad JSON", "[load][error]") {
    remove_saved_json();
    remove_apidata_file();

    {
        std::ofstream ofs("../saved.json");
        REQUIRE(ofs.is_open());
        ofs << "{ not valid json ";
    }
    write_apidata_file(0, 0);

    int tc=0, tp=0, ps=0, pc=0, fr=0;
    std::vector<float> psv = {};
    JsonFileManager mgr(tc, tp, ps, pc, fr, psv);

    REQUIRE_THROWS_AS(mgr.loadJsonFile(), std::runtime_error);

    remove_saved_json();
    remove_apidata_file();
}

TEST_CASE("saveJsonFile() throws on missing initial file", "[save][error]") {
    remove_saved_json();
    remove_apidata_file();

    int tc=0, tp=0, ps=1, pc=2, fr=0;
    std::vector<float> psv = {344, 433, 602, 682};
    JsonFileManager mgr(tc, tp, ps, pc, fr, psv);
    REQUIRE_THROWS_AS(mgr.saveJsonFile(), std::runtime_error);
}
TEST_CASE("saveJsonFile overwrites existing values", "[json][save][overwrite]") {
    {
        std::ofstream out("../saved.json");
        out << R"({
            "placedSpruces": 3,
            "placedCherryBlossomTrees": 4,
            "availablePos": [172, 258, 344, 433, 512, 602, 682],
            "positions": [172, 258, 344, 433, 512, 602, 682],
            "forests": 1
        })";
        out.close();
    }
    write_apidata_file(85, 30);

    int dummyTotalCommits = 0;
    int dummyTotalPrs = 0;
    placedSpruces = 5;
    placedCherryBlossomTrees = 7;
    forests = 2;

    std::vector<float> positions = {172, 344, 433, 602, 682};
    std::vector<float> expected = {172, 344, 433, 602, 682};

    JsonFileManager jsonFileManager(dummyTotalCommits, dummyTotalPrs, placedSpruces, placedCherryBlossomTrees, forests, positions);
    REQUIRE_NOTHROW(jsonFileManager.saveJsonFile());

    placedSpruces = 0;
    placedCherryBlossomTrees = 0;
    forests = 0;
    positions.clear();

    JsonFileManager jsonFileManagerReload(dummyTotalCommits, dummyTotalPrs, placedSpruces, placedCherryBlossomTrees, forests, positions);
    REQUIRE_NOTHROW(jsonFileManagerReload.loadJsonFile());

    REQUIRE(placedSpruces == 5);
    REQUIRE(placedCherryBlossomTrees == 7);

    REQUIRE(checkVector(positions, expected) == true);

    REQUIRE(forests == 2);
}
TEST_CASE("refresh actually refreshing the vector", "[refresh]") {
    remove_saved_json();
    remove_apidata_file();

    write_json_file(0, 0, 0, {});
    write_apidata_file(0, 0);

    std::vector<float> positions = {};

    JsonFileManager mgr(totalCommits, totalPrs, placedSpruces, placedCherryBlossomTrees, forests, positions);
    REQUIRE_NOTHROW(mgr.loadJsonFile());

    positions.clear();
    REQUIRE_NOTHROW(mgr.refresh());

    std::vector<float> expected = {172, 258, 344, 433, 512, 602, 682};
    REQUIRE(checkVector(positions, expected) == true);
}
TEST_CASE("refresh returning nothing if vector is not empty", "[refresh][return]") {
    remove_saved_json();
    remove_apidata_file();

    write_json_file(0, 0, 0, {172});
    write_apidata_file(10, 0);

    std::vector<float> positions = {};

    JsonFileManager mgr(totalCommits, totalPrs, placedSpruces, placedCherryBlossomTrees, forests, positions);
    REQUIRE_NOTHROW(mgr.loadJsonFile());

    REQUIRE_NOTHROW(mgr.refresh());
    REQUIRE(positions.size() == 1);
    REQUIRE(positions[0] == 172);
}