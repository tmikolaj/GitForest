#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include "pugixml.hpp"

class Sprite {
private:
    std::string m_path;
public:
    explicit Sprite(std::string path);
    ~Sprite() = default;

    void instantiate(float x, float y, float w, float h);
};

#endif //SPRITE_H
