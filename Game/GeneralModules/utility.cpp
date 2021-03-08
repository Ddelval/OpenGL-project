#include "GeneralModules/utility.hpp"

int viewPortWidth;
int viewPortHeight;

std::ostream& operator<<(std::ostream& o, glm::vec3 v) {
    o << v.x << " " << v.y << " " << v.z;
    return o;
}