#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "GeneralModules/Camera.hpp"
#include "GameCubes/Map.hpp"

namespace Entities {


class Player {
   private:
    Camera c;
    glm::vec3 velocity;
    glm::vec3 aceleration;
    glm::vec3 box;
    glm::vec3 centerPosition;
    float  jumpAccel = 5000;
    int remainingJumps = 0;
    int totalJumps = 1;
    GameModel::Map* map;

    static  float maxQuickness;
    static  float maxSpeed;
    

   public:
    Player(GameModel::Map* map);

    void recalculatePosition(float deltaTime);
    void setVelocity(glm::vec3 velocity);
    void addAceleration(glm::vec3 aceleration);
    glm::vec3 getViewPosition();
    glm::mat4 getCameraProyection();
    glm::vec3 getFront();
    void jump();
    void processMouse(float xoffset, float yoffset);

   private:
    bool checkPosition(glm::vec3& position);
};
}  // namespace Entities

#endif