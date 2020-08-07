#include "Player.hpp"

using namespace Entities;
Player::Player(GameModel::Map* map) {
    centerPosition = glm::vec3(10, 30, 10);
    jumpAccel = 120;
    this->map = map;
    box = glm::vec3(1, 2.5, 1);
    c = Camera(centerPosition + glm::vec3(0, 3*box.y / 4, 0), glm::vec3{0, 1, 0}, -90, 0);
    velocity = glm::vec3(0, 0, 0);
}
float Player::maxQuickness = 50;
float Player::maxSpeed = 35;
int jcount = 0;
float gravity = 4.9f;

float accumDelta = 0;
void Player::recalculatePosition(float deltaTime) {
    this->aceleration += glm::vec3(0, -gravity, 0);
    float div = 1;
    div = glm::max(div, glm::abs(this->aceleration.x / maxQuickness));
    div = glm::max(div, glm::abs(this->aceleration.z / maxQuickness));
    this->aceleration.x /= div;
    this->aceleration.z /= div;
    this->velocity += this->aceleration * deltaTime;

    div = 1;
    div = glm::max(div, glm::abs(this->velocity.x / maxSpeed));
    div = glm::max(div, glm::abs(this->velocity.z / maxSpeed));
    this->velocity.x /= div;
    this->velocity.z /= div;

    glm::vec3 position;
    glm::vec3 possibles[] = {
        this->velocity,
        {0, this->velocity.y, this->velocity.z},
        {this->velocity.x, this->velocity.y, 0},
        {this->velocity.x, 0, this->velocity.z},
        {this->velocity.x, 0, 0},
        {0, this->velocity.y, 0},
        {0, 0, this->velocity.z}};

    bool valid = false;
    for (int i = 0; i < 7; ++i) {
        position = this->centerPosition + possibles[i] * deltaTime;
        if (checkPosition(position)){
            valid = true;
            velocity = possibles[i];
            break;
        }
    }

    if (!valid) {
        remainingJumps = totalJumps;
        velocity = glm::vec3(0, 0, 0);
        aceleration = glm::vec3(0, 0, 0);
        return;
    }
    //std::cout << "yvel" << velocity.y << " count:" << jcount << std::endl;
    if (velocity.y == 0) {
        jcount++;
        if(jcount==2){
            this->aceleration.y = 0;
            remainingJumps = totalJumps;
            jcount = 0;
        }

    } else {
        jcount = 0;
    }

    float factor = std::pow(0.90,  (60 * deltaTime));
    std::cout << factor << std::endl;
    this->centerPosition = position;
    c.setPostion(this->centerPosition + glm::vec3(0, 3*box.y / 4.0f, 0));
        this->aceleration.x *=factor;
        this->aceleration.z *=factor;
        this->aceleration.y *= factor;
        this->velocity.x *= factor;
        this->velocity.z *= factor;
        this->velocity.y *= factor;
    if (glm::length(this->aceleration) < 0.5) this->aceleration = glm::vec3(0, 0, 0);
    if (glm::length(this->velocity) < 0.5) this->velocity = glm::vec3(0, 0, 0);
 }
void Player::jump(){
    if (remainingJumps) {
        this->aceleration += glm::vec3(0, jumpAccel, 0);
        remainingJumps--;
    }
}
bool Player::checkPosition(glm::vec3& position) {
    for (float i = -1; i < 2; i += 2) {
        for (float j = -1; j < 2; j += 2) {
            for (float w = -1; w < 2; w += 2) {
                glm::vec3 vertex = position + glm::vec3(i, j, w) * box / 2.0f;
                if (!map->checkEmtpySpace(vertex.x, vertex.y, vertex.z)) {
                    //std::cout << "not valid" << std::endl;
                    return false;
                }
            }
        }
    }
    return true;
}
void Player::setVelocity(glm::vec3 velocity) {
    this->velocity = velocity;
}

void Player::addAceleration(glm::vec3 acelerationDelta) {
    this->aceleration += acelerationDelta;
}

glm::vec3 Player::getViewPosition() {
    return this->centerPosition + glm::vec3(0, 3*box.y / 4, 0);
}

glm::mat4 Player::getCameraProyection() {
    return this->c.getWiewMatrix();
}
glm::vec3 Player::getFront() {
    return c.Front;
}
void Player::processMouse(float xoffset, float yoffset) {
    this->c.processMouseMovement(xoffset, yoffset);
}
