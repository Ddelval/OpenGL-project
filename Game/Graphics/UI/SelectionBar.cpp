#include "Graphics/UI/SelectionBar.hpp"

#include "GameCubes/DataDictionary.hpp"
#include "Graphics/MeshBuilder.hpp"

using namespace Graphics::UI;
using namespace Graphics;

float mat[16] = {
    1, 0, 0, 0,
    0, 8 / 6.0f, 0, 0,
    0, 0, 1, 0};

SelectionBar::SelectionBar() {
    std::string name = "BarFrame";
    const Texture* frame = DataDictionary::getTexture(name);
    glm::vec3 offset = {(2 - barWidth - sep * (size - 1)) / 2 - 1, -0.95, 0};
    generalScale = glm::make_mat4x4(mat) * (barWidth / size);
    for (int i = 0; i < size; ++i) {
        frames[i] = MeshBuilder::buildQuad(glm::vec3(), generalScale, frame);

        offsets[i] = offset;
        offset += glm::vec3(barWidth / size + sep, 0, 0);
    }
    shad = Shader("shaders/ui_vertex.glsl", "shaders/ui_fragment.glsl");

    for (int i = 0; i < size; ++i) {
        blocks[i] = nullptr;
        scales[i] = 1;
    }
}

void SelectionBar::draw() {
    float hei = (barWidth / size) * 8 / 6.0f;
    float wid = barWidth / size;
    updateScales();
    shad.use();
    glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i = 0; i < size; i++) {
        float scale = sin(scales[i] - 1) + 1;
        shad.setvec3(
            "offset", offsets[i] - glm::vec3{wid * (scale - 1) / 2, hei * (scale - 1) / 2, 0});
        shad.setmat4("scale", glm::mat4(glm::mat2(1) * scale));
        if (blocks[i] != nullptr) items[i].draw(shad);
        frames[i].draw(shad);
    }
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);
}
void SelectionBar::updateScales() {
    for (int i = 0; i < size; ++i) {
        scales[i] -= 0.04;
        if (scales[i] < 1) scales[i] = 1;
    }
    scales[selecteditem] += 0.06;
    if (scales[selecteditem] > 1.2) scales[selecteditem] = 1.2;
}

void SelectionBar::setSelected(int index) {
    this->selecteditem = index;
}

void SelectionBar::setContent(int slot, const GameModel::Cube* block, int quantity) {
    blocks[slot] = block;
    quantities[slot] = quantity;

    if (block != nullptr) items[slot] = MeshBuilder::buildQuad(glm::vec3(), generalScale, block->getCubeImage());
}