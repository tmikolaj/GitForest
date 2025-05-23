cmake_minimum_required(VERSION 3.30)
project(GitForest)

set(CMAKE_CXX_STANDARD 20)

# Adding pugixml source
set(PUGIXML_SRC external-libraries/pugixml/pugixml.cpp)

add_executable(GitForest main.cpp ${PUGIXML_SRC}
        src/Sprite.cpp
        src/Sprite.h
        src/Manager.cpp
        src/Manager.h
        src/JsonFileManager.cpp
        src/JsonFileManager.h)

# Including the pugixml directory
target_include_directories(GitForest PRIVATE external-libraries/pugixml)

# #Including the rapidjson directory
target_include_directories(GitForest PRIVATE external-libraries/rapidjson)