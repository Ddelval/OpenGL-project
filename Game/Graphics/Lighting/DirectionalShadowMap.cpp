
#include "Graphics/Lighting/DirectionalShadowMap.hpp"

#include <climits>
#include <functional>
#include <limits>

#include "GameCubes/Chunk.hpp"

using namespace Lighting;

DirectionalShadowMap::DirectionalShadowMap(int offsetX, int offsetY, Shader& s)
    : offsetX(offsetX), offsetY(offsetY), shad(s) {
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    this->fbuffer = std::make_shared<Graphics::FrameBuffer>();
    this->fbuffer->activate();
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureId, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
    }
    this->fbuffer->deactivate();
}

void DirectionalShadowMap::setDirections(glm::vec3 direction, glm::vec3 center, float spacing, float height) {
    this->direction = direction;
    this->spacing = spacing;
    this->center = center;
    this->height = height;

    this->direction /= std::abs(this->direction.y);
}

void DirectionalShadowMap::generateMap(Graphics::CameraData cam, std::vector<glm::vec3>& visiblePoints, std::function<void(void)> toDraw) {
    shad.use();
    glViewport(offsetX * resolution, offsetY * resolution, resolution, resolution);
    this->fbuffer->activate();
    glEnable(GL_SCISSOR_TEST);
    glScissor(offsetX * resolution, offsetY * resolution, resolution, resolution);

    glClear(GL_DEPTH_BUFFER_BIT);
    this->lightSpace = Graphics::CameraData::generateOrthogonalProyection(visiblePoints, this->direction);

    shad.setmat4("lightSpaceMatrix", lightSpace);
    shad.setmat4("model", glm::mat4(1));
    toDraw();

    glDisable(GL_SCISSOR_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectionalShadowMap::activateMap(Shader& s) const {
    glActiveTexture(GL_TEXTURE15);
    glBindTexture(GL_TEXTURE_2D, this->textureId);

    s.setmat4("lightSpaceMatrix", this->lightSpace);
}
uint DirectionalShadowMap::getTexture() const {
    return this->textureId;
}

Shader DirectionalShadowMap::getShader() const {
    return this->shad;
}
void DirectionalShadowMap::setDrawable(Graphics::Drawable* mesh) {
    this->mesh = mesh;
}
glm::mat4 DirectionalShadowMap::getLightSpace(Graphics::CameraData) const {
    return this->lightSpace;
}