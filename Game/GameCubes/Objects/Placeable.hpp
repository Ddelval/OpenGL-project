#ifndef PLACEABE_MODEL_HPP
#define PLACEABE_MODEL_HPP

#include "GameCubes/Cube.hpp"
#include <functional>
namespace GameModel {
class Placeable {
   private:
    static constexpr int max_width = 17;
    static constexpr int max_length = 17;
    static constexpr int max_height = 33;
    static constexpr int mid_width = max_width / 2 + 1;
    static constexpr int mid_height = max_height / 2 + 1;
    static constexpr int mid_length = max_length / 2 + 1;

    const GameModel::Cube* data[max_width][max_height][max_length];
    
    public:
     Placeable();
     void drawTree();
     const Cube*& atCoordinates(int x, int y, int z);
     void forEach(std::function<void(int, int, int, const Cube*)> fun);
};
};  // namespace GameModel
#endif
