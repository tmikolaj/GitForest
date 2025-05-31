add_executable(GitForest_tests
        catch2/catch_amalgamated.cpp
        process_func_test.cpp
        load_save_refresh_json_func_test.cpp
        globals.h
        globals.cpp
        ../src/JsonFileManager.h
        ../src/JsonFileManager.cpp
)

target_include_directories(GitForest_tests PRIVATE catch2)

target_compile_features(GitForest_tests PRIVATE cxx_std_20)

add_test(NAME GitForestTests COMMAND GitForest_tests)