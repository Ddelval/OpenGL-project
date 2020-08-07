#ifndef UTILITY_HPP
#define UTILITY_HPP


#include <glad/glad.h>


#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <math.h>
typedef unsigned int uint;

std::ostream& operator<<(std::ostream& o, glm::vec3 v);

#endif