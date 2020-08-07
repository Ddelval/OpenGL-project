#include "GeneralModules/utility.hpp"
std::ostream& operator<<(std::ostream& o, glm::vec3 v) {
    o << v.x << " " << v.y << " " << v.z;
    return o;
}