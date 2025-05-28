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
    pugi::xml_node mainGroup = background.child("svg").child("g");

    pugi::xml_node ref = mainGroup.first_child();
    for (int i = 0; i < 9; i++) {
        ref = ref.next_sibling();
    }

    pugi::xml_node spriteGroup;
    if (ref) {
        spriteGroup = mainGroup.insert_child_after("g", ref);
    } else {
        spriteGroup = mainGroup.append_child("g");
    }

    std::string transformStr = "translate(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    spriteGroup.append_attribute("transform") = transformStr.c_str();

    pugi::xml_node elementRoot = element.child("svg");
    for (pugi::xml_node child : elementRoot.children()) {
        if (child.type() == pugi::node_element) {
            spriteGroup.append_copy(child);
        }
    }
    background.save_file("../assets/forest.svg");
}