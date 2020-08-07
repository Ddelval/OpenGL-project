#include "GeneralModules/Camera.hpp"

#include <math.h>

#include <algorithm>
#include <cmath>

using v3 = glm::vec3;
using m4 = glm::mat4;
Camera::Camera() {

}
Camera::Camera(v3 position, v3 up, float yaw, float pitch) {
    this->yaw = yaw;
    this->pitch = pitch;
    this->WorldUp = up;
    this->Position = position;
    updateCameraVectors();
}

m4 Camera::getWiewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::move(CameraMov direction, float delta) {
    v3 dx;
    switch (direction) {
        case CameraMov::FWD:
            dx = this->Front;
            break;
        case CameraMov::BCK:
            dx = -this->Front;
            break;
        case CameraMov::LFT:
            dx = -this->Right;
            break;
        case CameraMov::RGH:
            dx += this->Right;
            break;
    }
    Position += dx * (this->movementSpeed * delta);
}

void Camera::setPostion(glm::vec3 position){
    this->Position= position;
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
    xoffset *= this->mouseSensitivity;
    yoffset *= this->mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;
    pitch = std::min(abs(pitch), 89.0f) * (2 * (pitch > 0) - 1);
    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    v3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}