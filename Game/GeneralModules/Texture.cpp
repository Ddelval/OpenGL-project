#include "GeneralModules/Texture.hpp"

#include "GeneralModules/inter.h"
#define STB_IMAGE_IMPLEMENTATION
#include <string>

#include "External/stb_image.h"

Texture::Texture(const char* name) {
    this->name = std::string(name);

    int wid, hei, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(name, &wid, &hei, &nrChannels, 0);

    glGenTextures(1, &this->id);

    glBindTexture(GL_TEXTURE_2D, this->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    unsigned int typ, typ2;
    if (nrChannels == 3)
        typ = GL_SRGB;
    else
        typ = GL_SRGB_ALPHA;
    if (nrChannels == 3)
        typ2 = GL_RGB;
    else
        typ2 = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, typ, wid, hei, 0, typ2, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

bool Texture::operator==(const Texture& t) const {
    return this->id == t.id;
}

bool Texture::operator<(const Texture& t) const {
    return this->id < t.id;
}

void Texture::activateTexture(int buffer) const {
    glActiveTexture(GL_TEXTURE0 + buffer);
    glBindTexture(GL_TEXTURE_2D, this->id);
}