#include "Graphics/FrameBuffer.hpp"

using namespace Graphics;

FrameBuffer::FrameBuffer(){
    glGenFramebuffers(1, &id);
}

FrameBuffer::~FrameBuffer(){
    glDeleteFramebuffers(1, &id);
}

void FrameBuffer::activate() const {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FrameBuffer::deactivate() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
