#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP
#include "GeneralModules/Shader.hpp"
#include "Graphics/CameraData.hpp"
namespace Graphics {
class Drawable {
   public:
    virtual void draw( Shader& s) = 0;
    virtual void draw( Shader& s, CameraData& c) = 0;
    virtual void draw(Shader& s, std::vector<glm::vec3>& d) = 0;
};
}  // namespace Graphics

#endif