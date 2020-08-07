#include "GameCubes/Cube.hpp"
#include "Graphics/MeshBuilder.hpp"

using namespace GameModel;

Cube::Cube(std::array<const Texture*, 6>& arr, const Texture* cubeImage) {
    faces = Graphics::MeshBuilder::buildCube(glm::vec3(0,0,0),glm::mat4(1),arr);
    this->cubeImage = cubeImage;
}

const Graphics::Mesh* Cube::getFace(CubeFace cf) const{
    return &faces[static_cast<int>(cf)];
}

void Cube::setTransparent(bool value){
    this->transparent = value;
}

bool Cube::isTransparent() const {
    return this->transparent;
}

const Texture* Cube::getCubeImage() const{
    return this->cubeImage;
}