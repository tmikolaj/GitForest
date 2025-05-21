#include <iostream>
#include "pugixml.hpp"

int main() {
    pugi::xml_document doc;
    doc.load_file("Background.svg");
    if (!doc) {
        std::cerr << "Failed to load background.svg" << std::endl;
    } else {
        std::cout << "loaded";
    }
}