#ifndef CAMERA_DATA_HPP
#define CAMERA_DATA_HPP
#include "GeneralModules/utility.hpp"
#include <vector>
namespace Graphics {

class CameraData {
   public:
    glm::vec2 pos;
    glm::vec2 dir;
    float drawingDistance;
    float hangle;
    float vangle;
    float hcos;
    float vcos;

   public:
    CameraData(){};
    CameraData(glm::vec2 pos, glm::vec2 dir, float drawingDistance);
    void setVAngle(float vangle);
    void setHAngle(float hangle);
    void setAngles(float vangle, float ratio);

    static glm::mat4 generateOrthogonalProyection(std::vector<glm::vec3>& points, glm::vec3 direction);

};

}  // namespace Graphics

#endif