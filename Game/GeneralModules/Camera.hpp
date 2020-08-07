#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "GeneralModules/utility.hpp"

enum class CameraMov {
    FWD, BCK, LFT, RGH
};

class Camera{
    public:
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;
        
        float yaw;
        float pitch;
        
        float movementSpeed=5;
        float mouseSensitivity=0.05f;
        
        
        Camera();
        Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
        
        glm::mat4 getWiewMatrix();
        
        void move(CameraMov direction, float  deltaTime);
        void setPostion(glm::vec3 position);

        void processMouseMovement(float xoffset, float yoffset);

    private:
        void updateCameraVectors();
};


#endif