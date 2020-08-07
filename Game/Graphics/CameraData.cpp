#include "Graphics/CameraData.hpp"

#include <vector>
#include <algorithm>
using namespace Graphics;
CameraData::CameraData(glm::vec2 pos, glm::vec2 dir, float drawingDistance)
    : pos{(pos)}, dir{glm::normalize(dir)}, drawingDistance{drawingDistance} {
}

void CameraData::setHAngle(float hangle) {
    this->hangle = hangle;
    this->hcos = cos(hangle);
}
void CameraData::setVAngle(float vangle) {
    this->vangle = vangle;
    this->vcos = cos(vangle);
}
void CameraData::setAngles(float vangle, float ratio) {
    float hangle = 2 * glm::atan(ratio * glm::tan(vangle / 2));
    setHAngle(hangle);
    setVAngle(vangle);
}

glm::mat4 CameraData::generateOrthogonalProyection(std::vector<glm::vec3>& points, glm::vec3 direction) {
    glm::vec3 d = glm::normalize(direction);
    glm::vec3 u = glm::normalize(glm::cross(d, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 v = glm::normalize(glm::cross(d, u));

    glm::vec3 base[] = {u, v, d};
    float mins[3];
    float maxs[3];
    for (int i = 0; i < 3; ++i) {
        mins[i] = std::numeric_limits<float>::max();
        maxs[i] = std::numeric_limits<float>::min();
    }
    for (auto pos : points) {
        for (int j = 0; j < 3; ++j) {
            float proy = glm::dot(pos, base[j]);
            mins[j] = std::min(mins[j], proy);
            maxs[j] = std::max(maxs[j], proy);
        }
    }

    glm::vec3 camCenter = {0.0f, 0.0f, 0.0f};
    for (int i = 0; i < 3; ++i) camCenter += base[i] * ((maxs[i] + mins[i]) / 2);

    glm::vec3 camPosition = camCenter;
    camPosition += d * ((maxs[2] - mins[2]) / 2);

    float distances[3];
    for (int i = 0; i < 3; ++i) distances[i] = (maxs[i] - mins[i]) / 2;

    glm::mat4 lightPro = glm::ortho(-distances[0], distances[0], -distances[1], distances[1], 0.0f, 2 * distances[2]);
    glm::mat4 lightView = glm::lookAt(camPosition, camCenter, glm::vec3(0, 1.0f, 0));
    return lightPro * lightView;
}

