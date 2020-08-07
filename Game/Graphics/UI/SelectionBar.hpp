#ifndef _SELECTION_BAR_HPP
#define _SELECTION_BAR_HPP

#include "GameCubes/Cube.hpp"
#include "GeneralModules/utility.hpp"

namespace Graphics::UI {
class SelectionBar {
   private:
    static constexpr int size = 8;
    float barWidth = 1;
    float sep = 0.01;
    const GameModel::Cube* blocks[size];
    int selecteditem = 1;
    glm::mat4 generalScale;
    glm::vec3 offsets[size];
    float scales[size];
    int quantities[size];
    Mesh frames[size];
    Mesh items[size];
    Shader shad;

   public:
    SelectionBar();
    void draw();
    void setSelected(int pos);
    void setContent(int slot, const GameModel::Cube* block, int quantity);

   private:
    void updateScales();
    
};
};  // namespace Graphics::UI

#endif