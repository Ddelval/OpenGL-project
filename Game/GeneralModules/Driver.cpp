
#include <iostream>
#include "inter.h"
//#include <unistd.h>

void framebuffer_size_callback(GLFWwindow* window, int wid,int hei){
    glViewport(0,0,wid,hei);
}

extern uint shaderProgram;
int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _APPLE_
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    
    int wid = 800;
    int hei = 600;
#ifdef _APPLE_
    std::cout << "HI" << std::endl;
    wid *= 2;
    hei *= 2;
#endif
    GLFWwindow* window= glfwCreateWindow(wid,hei,"LearnOpenGL",NULL,NULL);
    if(window== NULL){
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, processMouse);
    glfwSetScrollCallback(window, processScroll);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    }
    glfwSwapInterval(1);


    glfwGetFramebufferSize(window,&wid, &hei);
    std::cout << wid << " " << hei << std::endl;
    //glViewport(0,0,wid,hei);
    setup(); 
    glEnable(GL_DEPTH_TEST);

        float fprev = glfwGetTime();
    
    while(!glfwWindowShouldClose(window)){
        std::cout << 1 / (glfwGetTime() - fprev) << std::endl;
        fprev = glfwGetTime();

        processInput(window);
        
        glClearColor(0.0f,0.3f,0.7f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw();
        
        glfwSwapBuffers(window);
        
        glfwPollEvents();
        
    }
    
    glfwTerminate();
    return 0;
}