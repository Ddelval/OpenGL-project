#ifndef DIRECTIONAL_SHADOW_MAP_HPP
#define DIRECTIONAL_SHADOW_MAP_HPP

#include "Graphics/Drawable.hpp"
#include "Graphics/FrameBuffer.hpp"
#include <functional>
namespace Lighting {
class DirectionalShadowMap {
   private:
    glm::vec3 direction;
    glm::vec3 center;
    float height;
    float spacing;
    glm::mat4 lightSpace;

    Graphics::Drawable* mesh;
    int offsetX;  //Horizontal index of this map in the atlas
    int offsetY;  //Vertical index of this map in the atlas
    static const int resolution = 2048;
    uint textureId;
    std::shared_ptr<Graphics::FrameBuffer> fbuffer;
    Shader shad;
    uint pbo;

   public:
    DirectionalShadowMap(int offsetX, int offsetY, Shader& s);
    DirectionalShadowMap() = default;

    void setDirections(glm::vec3 direction, glm::vec3 center, float spacing, float height);
    void generateMap(Graphics::CameraData cam, std::vector<glm::vec3>& visiblePoints, std::function<void(void)> toDraw);
    glm::mat4 getLightSpace(Graphics::CameraData) const;

    uint getTexture() const;
    Shader getShader() const;
    void activateMap(Shader& s) const;
    void setDrawable(Graphics::Drawable*);

   private:
    void generateLightSpace(Graphics::CameraData cam);
};
}  // namespace Lighting

#endif