#include "Sprite.h"

static pugi::xml_document background;

Sprite::Sprite(std::string path) : m_path(path) {
    element.load_file(m_path.c_str());
    background.load_file("../assets/forest.svg");
}
// copy constructor
Sprite::Sprite(const Sprite &other) : Sprite(other.m_path) {
    element.load_file(m_path.c_str());
}
void Sprite::instantiate(float x, float y) {
    // Logic for instantiating a sprite
    pugi::xml_node backgroundRoot = background.child("svg");
    pugi::xml_node elementRoot = element.child("svg");

    pugi::xml_node group = backgroundRoot.append_child("g");

    std::string transformStr = "translate("+ std::to_string(x) + ", "+ std::to_string(y) + ")";
    group.append_attribute("transform") = transformStr.c_str();

    for (pugi::xml_node child : elementRoot.children()) {
        group.append_copy(child);
    }
    background.save_file("../assets/forest.svg");
}