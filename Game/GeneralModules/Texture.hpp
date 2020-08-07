#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <string>

#include "utility.hpp"

class Texture {
   public:
    uint id;
    std::string name;

    Texture() : id{0} {};
    Texture(const char* imagname);

    bool operator == (const Texture& t) const;
    bool operator < (const Texture& t) const;

    void activateTexture(int buffer) const;
};
#endif