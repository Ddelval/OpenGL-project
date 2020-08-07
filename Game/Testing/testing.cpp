#include <stdlib.h>

#include "Graphics/UI/SelectionBar.hpp"
#include <array>
#include <cmath>
#include <filesystem>
#include <iomanip>
#include <vector>

#include "Entity/Player.hpp"
#include "GameCubes/Chunk.hpp"
#include "GameCubes/Cube.hpp"
#include "GameCubes/DataDictionary.hpp"
#include "GameCubes/Map.hpp"
#include "GeneralModules/Camera.hpp"
#include "Graphics/Lighting/DirectionalShadowMap.hpp"
#include "Graphics/MeshBuilder.hpp"

#include "GeneralModules/Shader.hpp"
#include "GeneralModules/Texture.hpp"
#include "GeneralModules/inter.h"

static Shader s2;

static float mi = 0;
using namespace std;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

GameModel::Map map;
Entities::Player player(&map);

Graphics::Mesh breaking;
bool isBreaking = false;
glm::vec3 prevBlock = {-1, -1, -1};
int counterBlock = 60;

bool cursorHid= true;
bool pressed = false;
void processInput(GLFWwindow* window) {
    deltaTime = glfwGetTime() - lastFrame;
    lastFrame = glfwGetTime();

    float speed = 7.0f;
    glm::vec3 vel(0, 0.0f, 0);
    glm::vec3 front = speed * glm::normalize(player.getFront());
    glm::vec3 left = speed * glm::normalize(glm::cross(glm::vec3(0, 1, 0), front));
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        vel += front;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        vel -= front;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vel += left;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vel -= left;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        player.jump();
        //cout << "hi";
    }
       
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        if (!pressed) {
            if(cursorHid)
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

            else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            cursorHid = !cursorHid;
        }
        pressed = true;
            }
        else{
            pressed = false;
        }
     isBreaking = false;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        glm::vec3 v = map.getFirstBlock(player.getViewPosition(), player.getFront(), 100);
        v.x = int(v.x);
        v.y = int(v.y);
        v.z = int(v.z);
        if (v.x >= 0 && v.y >= 0 && v.z >= 0)
            if (v == prevBlock) {
                counterBlock--;
                if (counterBlock == 0) {
                    map.updateBlock(v.x, v.y, v.z, nullptr);
                    counterBlock = 60;
                }
                
                std::string nam = "break_" + to_string(6 - int(counterBlock / 8.6f));

                const Texture* t = DataDictionary::getTexture(nam);
                std::array<const Texture*, 6> arr{t, t, t, t, t, t};
                breaking = Graphics::MeshBuilder::getOutterCube(v, arr);
                isBreaking = true;

            } else {
                counterBlock = 60;
                prevBlock = v;
            }

    } else {
        counterBlock = 60;
    }

    glm::vec3 acc = glm::vec3(vel.x, 0, vel.z);
    player.addAceleration(acc);
    player.recalculatePosition(deltaTime);
}

static float lastX = 400, lastY = 300;
static bool firstMouse = true;
void processMouse(GLFWwindow* window, double xpos, double ypos) {
    if (!cursorHid) {
        firstMouse = true;
        return;
    }
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    player.processMouse(xoffset, yoffset);

    lastX = xpos;
    lastY = ypos;
}
Graphics::UI::SelectionBar* sb;

float ipos = 0;
void processScroll(GLFWwindow* window, double xoffset, double yoffset) {
   // std::cout << yoffset << endl;
    ipos += (-yoffset / 3);
    if (ipos < 0) ipos += 8;
    if (ipos >= 8) ipos -= 8;
    sb->setSelected(int(ipos));
}

const uint shadowWid = 2 * 2048, shadowHei = 2048;

uint depthMap;


Shader s, sdebug;
void setup() {
    s2 = Shader("shaders/basic_vertex.glsl", "shaders/basic_fragment.glsl");

    s = Shader("shaders/Lighting_vertex.glsl", "shaders/Lighting_fragment.glsl");
    sdebug = Shader("shaders/debug_vertex.glsl", "shaders/debug_fragment.glsl");
    map = GameModel::Map(50, 1, 50, 100, s2, s);

    //map.addTree(24, 24);
    for (int i = 0; i < 10; ++i) {
        map.addTree(rand() / double(RAND_MAX) * 50, rand() / double(RAND_MAX) * 50);
    }
    sb = new Graphics::UI::SelectionBar;
}

void renderQuad();
bool b = true;
void draw() {
    cout << "draw frame" << endl;
   // sb->setContent(0, DataDictionary::getCube("Grass"), 1);
    glEnable(GL_FRAMEBUFFER_SRGB);
    s.tex = true;
    auto vp = player.getFront();
    auto vf = player.getViewPosition();

    Graphics::CameraData cd({vf.x, vf.z}, {vp.x, vp.z}, 150);
    cd.setAngles(glm::radians(45.0f), 800 / 600.0f);
    s.use();

    int wid = 400, hei = 300;
#ifdef _APPLE_
    wid *= 2;
    hei *= 2;
#endif
    
    glViewport(0, 0, wid, hei);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(0.0f, 0.3f, 0.7f, 1.0f);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 300.0f);
    s.setmat4("projection", projection);
    glm::mat4 model = glm::mat4(1.0);
    s.setmat4("model", model);
    s.setmat4("view", player.getCameraProyection());
    glActiveTexture(GL_TEXTURE15);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    s.setInt("shadowSampler", 15);

    s.setFloat("shininess", 8);
    s.setvec3("dirLight.direction", glm::normalize(glm::vec3(-5, -4, 2)));
    s.setvec3("dirLight.ambient", glm::vec3(0.3f, 0.2f, 0.2f));
    s.setvec3("dirLight.diffuse", glm::vec3(0.7f, 0.6f, 0.6f));
    s.setvec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
    s.setvec3("viewPos", player.getViewPosition());
    
    s.setmat3("normalMatrix", glm::mat3(glm::transpose(glm::inverse(model))));

    cd.drawingDistance = 175;
    map.draw(s, cd);

    if (isBreaking) breaking.draw(s);
    
    //sb->draw();
}
