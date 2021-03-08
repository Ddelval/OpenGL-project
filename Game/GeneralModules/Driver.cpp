
#include <iostream>

#include "inter.h"
//#include <unistd.h>

void framebuffer_size_callback(GLFWwindow* window, int wid, int hei) {
    glViewport(0, 0, wid, hei);
}

extern uint shaderProgram;
extern int viewPortWidth;
extern int viewPortHeight;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    int wid = 800;
    int hei = 600;

    viewPortWidth = wid;
    viewPortHeight = hei;
    
#ifdef __APPLE__
    viewPortWidth *= 2;
    viewPortHeight *= 2;
#else

#endif
    GLFWwindow* window = glfwCreateWindow(wid, hei, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, processMouse);
    glfwSetScrollCallback(window, processScroll);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "glad loading error" << std::endl;
    }

    glfwSwapInterval(1);

    setup();

    glEnable(GL_DEPTH_TEST);

    float fprev = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        std::cout << 1 / (glfwGetTime() - fprev) << std::endl;
        fprev = glfwGetTime();

        processInput(window);

        glClearColor(0.0f, 0.3f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}