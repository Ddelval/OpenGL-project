#ifndef CUBE_DIC_HPP
#define CUBE_DIC_HPP

#include "GameCubes/Cube.hpp"
#include "GeneralModules/Texture.hpp"
#include <string>

namespace DataDictionary{
    const GameModel::Cube* getCube(std::string name);
    const Texture* getTexture(std::string name);
}  

#endif