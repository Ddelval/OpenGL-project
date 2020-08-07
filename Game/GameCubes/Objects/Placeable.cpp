#include "GameCubes/Objects/Placeable.hpp"

#include "GameCubes/DataDictionary.hpp"
using namespace GameModel;
Placeable::Placeable() {
    for (int i = 0; i < max_width; ++i) {
        for (int j = 0; j < max_height; ++j) {
            for (int w = 0; w < max_length; ++w) {
                data[i][j][w] = nullptr;
            }
        }
    }
}
inline const Cube*& Placeable::atCoordinates(int x, int y, int z) {
    return data[mid_width + x][mid_height + y][mid_length + z];
}
void Placeable::drawTree() {
    int h = 4 + (double(rand()) / RAND_MAX) * 3;
    int r = 2 + (double(rand()) / RAND_MAX) * 3;

    for (int i = 0; i < h; ++i) {
        atCoordinates(0, i, 0) = DataDictionary::getCube("Wood");
    }
    for (int h2 = 0; h2 < 4;++h2) {
        int r = (4 - h2) / 2;
        for (int i = -r; i <= r; ++i) {
            for (int j = -r; j <= r;++j){
                atCoordinates(i, h2+h, j) = DataDictionary::getCube("Leaves");
            }
        }
    }
}

void Placeable::forEach(std::function<void(int, int, int, const Cube*)> fun) {
    for (int i = 0; i < max_width; ++i) {
        for (int j = 0; j < max_height; ++j) {
            for (int w = 0; w < max_length; ++w) {
                const Cube* d = data[i][j][w];
                int x = i - mid_width;
                int y = j - mid_height;
                int z = w - mid_length;
                fun(x, y, z, d);
            }
        }
    }
}