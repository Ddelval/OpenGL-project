#ifndef CUBE_GAME_MODEL_HPP
#define CUBE_GAME_MODEL_HPP
#include "Graphics/Mesh.hpp"
#include <array>
namespace GameModel {
enum class CubeFace { top,
                      bot,
                      left,
                      right,
                      front,
                      back };
class Cube {
   private:
    std::array<Graphics::Mesh, 6> faces;
    const Texture* cubeImage;
    bool transparent = 0;

   public:
    const Graphics::Mesh* getFace(CubeFace c) const;
    Cube() = default;
    Cube(std::array<const Texture*, 6>& arr, const Texture* cubeImage);
    void setTransparent(bool value);
    bool isTransparent() const;
    const Texture* getCubeImage() const;
};
}  // namespace GameModel

#endif