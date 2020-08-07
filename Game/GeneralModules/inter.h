#ifndef E1_H
#define E1_H
#include "utility.hpp"


void processMouse(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void processScroll(GLFWwindow* window, double xoffset, double yoffset);

void setup();

void draw();

#endif