#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include "pugixml.hpp"

class Sprite {
private:
    std::string m_path;
    pugi::xml_document element;
public:
    explicit Sprite(std::string path);
    Sprite(const Sprite& other); // copy constructor
    ~Sprite() = default;

    void instantiate(float x, float y);
};

#endif //SPRITE_H
