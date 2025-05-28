#define CATCH_CONFIG_MAIN
#include <fstream>
#include <cstdio> // std::remove (for file removing)
#include <string>
#include "../external-libraries/rapidjson/document.h"
#include "../external-libraries/rapidjson/prettywriter.h"
#include "../external-libraries/rapidjson/stringbuffer.h"
#include "../src/JsonFileManager.h"
#include "catch_amalgamated.hpp"

static void write_json_file(int spruce, int cherry) {
    rapidjson::Document doc;
    doc.SetObject();
    auto& alloc = doc.GetAllocator();
    doc.AddMember("placedSpruces", spruce, alloc);
    doc.AddMember("placedCherryBlossomTrees", cherry, alloc);

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

TEST_CASE("loadJsonFile() success path", "[load]") {
    remove_saved_json();
    write_json_file(3, 5);

    int totalCommits = 0, totalPrs = 0;
    int placedSpruces = -1, placedCherryBlossomTrees = -1;

    JsonFileManager jsonFileManager(totalCommits, totalPrs, placedSpruces, placedCherryBlossomTrees);

    REQUIRE_NOTHROW(jsonFileManager.loadJsonFile());
    REQUIRE(placedSpruces            == 3);
    REQUIRE(placedCherryBlossomTrees == 5);

    remove_saved_json();
}

TEST_CASE("saveJsonFile() overwrites correctly", "[save]") {
    remove_saved_json();

    write_json_file(0, 0);

    int totalCommits = 0, totalPrs = 0;
    int placedSpruces = 7, placedCherryBlossomTrees = 9;

    JsonFileManager jsonFileManager(totalCommits, totalPrs, placedSpruces, placedCherryBlossomTrees);

    REQUIRE_NOTHROW(jsonFileManager.saveJsonFile());

    rapidjson::Document doc = read_json_file();

    REQUIRE(doc["placedSpruces"].IsInt());
    REQUIRE(doc["placedCherryBlossomTrees"].IsInt());
    REQUIRE(doc["placedSpruces"].GetInt()            == 7);
    REQUIRE(doc["placedCherryBlossomTrees"].GetInt() == 9);

    remove_saved_json();
}

TEST_CASE("loadJsonFile() throws on missing file", "[load][error]") {
    remove_saved_json();

    int tc=0, tp=0, ps=0, pch=0;
    JsonFileManager mgr(tc,tp,ps,pch);
    REQUIRE_THROWS_AS(mgr.loadJsonFile(), std::runtime_error);
}

TEST_CASE("loadJsonFile() throws on bad JSON", "[load][error]") {
    remove_saved_json();

    {
        std::ofstream ofs("../saved.json");
        REQUIRE(ofs.is_open());
        ofs << "{ not valid json ";
    }

    int tc=0, tp=0, ps=0, pch=0;
    JsonFileManager mgr(tc,tp,ps,pch);
    REQUIRE_THROWS_AS(mgr.loadJsonFile(), std::runtime_error);

    remove_saved_json();
}

TEST_CASE("saveJsonFile() throws on missing initial file", "[save][error]") {
    remove_saved_json();

    int tc=0, tp=0, ps=1, pch=2;
    JsonFileManager mgr(tc,tp,ps,pch);
    REQUIRE_THROWS_AS(mgr.saveJsonFile(), std::runtime_error);
}
TEST_CASE("saveJsonFile overwrites existing values", "[json][save][overwrite]") {
    {
        std::ofstream out("../saved.json");
        out << R"({
            "placedSpruces": 3,
            "placedCherryBlossomTrees": 4
        })";
        out.close();
    }

    int dummyTotalCommits = 0;
    int dummyTotalPrs = 0;
    int placedSpruces = 2;
    int placedCherryBlossomTrees = 1;

    JsonFileManager jsonFileManager(dummyTotalCommits, dummyTotalPrs, placedSpruces, placedCherryBlossomTrees);
    jsonFileManager.saveJsonFile();

    placedSpruces = 0;
    placedCherryBlossomTrees = 0;

    JsonFileManager jsonFileManagerReload(dummyTotalCommits, dummyTotalPrs, placedSpruces, placedCherryBlossomTrees);
    jsonFileManagerReload.loadJsonFile();

    REQUIRE(placedSpruces == 2);
    REQUIRE(placedCherryBlossomTrees == 1);
}