#include "GameCubes/Chunk.hpp"

#include "GameCubes/DataDictionary.hpp"
#include "Graphics/MeshBuilder.hpp"

using namespace GameModel;

Chunk::Chunk() {
    for (int i = 0; i < ChunkWidth; ++i) {
        for (int j = 0; j < ChunkHeight; ++j) {
            for (int w = 0; w < ChunkLength; ++w) {
                data[i][j][w] = nullptr;
            }
        }
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j)
            adyacent[i][j] = nullptr;
    }
}

const Graphics::Mesh& Chunk::getMesh() const {
    return processedChunk;
}

void Chunk::setPosition(const glm::vec3& chunkPos) {
    this->chunkPos = chunkPos;
}

void Chunk::updateMesh() {
    std::vector<const Graphics::Mesh*> meshes;
    std::vector<glm::vec3> offsets;

    int nelements = ChunkWidth * ChunkLength * 2 + ChunkWidth * ChunkHeight * 2 + ChunkLength * ChunkHeight * 2;

    meshes.reserve(nelements);
    offsets.reserve(nelements);

    GameModel::CubeFace faces[6];
    int ecount;

    int coord[3];
    CubeFace order[3][2] = {
        {CubeFace::front, CubeFace::back},
        {CubeFace::bot, CubeFace::top},
        {CubeFace::left, CubeFace::right}};

    for (int i = 0; i < ChunkWidth; ++i) {
        for (int j = 0; j < ChunkHeight; ++j) {
            for (int w = 0; w < ChunkLength; ++w) {
                if (data[i][j][w] == nullptr) continue;
                ecount = 0;
                if (data[i][j][w]->isTransparent()) {
                    for (int f = 0; f < 6; ++f) {
                        faces[ecount++] = static_cast<CubeFace>(f);
                    }
                    goto INSERT;
                }
                coord[0] = i;
                coord[1] = j;
                coord[2] = w;
                for (int i = 0; i < 3; ++i) {
                    coord[i]--;
                    if (coord[i] == -1) {
                        int b = coord[i];
                        coord[i] = dimensions[i] - 1;
                        if (adyacent[i][0] != nullptr && adyacent[i][0]->data[coord[0]][coord[1]][coord[2]] == nullptr) {
                            faces[ecount++] = order[i][0];
                        }
                        coord[i] = b;
                    } else if (data[coord[0]][coord[1]][coord[2]] == nullptr) {
                        faces[ecount++] = order[i][0];
                    }

                    coord[i] += 2;
                    if (coord[i] == dimensions[i]) {
                        int c = coord[i];
                        coord[i] = 0;
                        if (adyacent[i][1] != nullptr && adyacent[i][1]->data[coord[0]][coord[1]][coord[2]] == nullptr) {
                            faces[ecount++] = order[i][1];
                        }
                        coord[i] = c;
                    } else if (data[coord[0]][coord[1]][coord[2]] == nullptr) {
                        faces[ecount++] = order[i][1];
                    }
                    coord[i]--;
                }
            INSERT:
                for (int f = 0; f < ecount; ++f) {
                    meshes.push_back(data[i][j][w]->getFace(faces[f]));
                    offsets.push_back(glm::vec3(i, j, w) + this->chunkPos);
                }
            }
        }
    }
    if (meshes.size() != 0) {
        this->processedChunk = Graphics::MeshBuilder::joinMesh(meshes, &offsets);
    }
    modified = false;
}

void Chunk::setSurroundings(Chunk* Xprev, Chunk* Xnext, Chunk* Yprev, Chunk* Ynext, Chunk* Zprev, Chunk* Znext) {
    adyacent[0][0] = Xprev;
    adyacent[0][1] = Xnext;
    adyacent[1][0] = Yprev;
    adyacent[1][1] = Ynext;
    adyacent[2][0] = Zprev;
    adyacent[2][1] = Znext;
}

void Chunk::draw(const Shader& s) {
    if (modified) this->updateMesh();
    this->processedChunk.draw(s);
}

void Chunk::setCube(int x, int y, int z, const Cube* cub) {
    this->data[x][y][z] = cub;
    modified = true;
}

void Chunk::updateCube(int x, int y, int z, const Cube* cub) {
    this->data[x][y][z] = cub;
    modified = true;
    std::cout << "blah " << x << " " << y << " " << z << std::endl;
    if (adyacent[0][0] && x == 0) adyacent[0][0]->modified = true;
    if (adyacent[0][1] && x == ChunkWidth - 1) adyacent[0][1]->modified = true;
    if (adyacent[1][0] && y == 0) adyacent[1][0]->modified = true;
    if (adyacent[1][1] && y == ChunkHeight - 1) adyacent[1][1]->modified = true;
    if (adyacent[2][0] && z == 0) adyacent[2][0]->modified = true;
    if (adyacent[2][1] && z == ChunkLength - 1) adyacent[2][1]->modified = true;
}

void Chunk::setHeight(int x, int z, int h) {
    this->heightMap[x][z] = h;
}

bool Chunk::freePosition(float x, float y, float z) {
    if (x > ChunkWidth || y > ChunkHeight || z > ChunkLength) {
        std::cerr << "Out of bounds" << std::endl;
        return false;
    }

    return data[int(x)][int(y)][int(z)] == nullptr;
}

void Chunk::addTree(const int x, const int z) {
    for (int i = 0; i < 7; ++i) {
        data[x][(int)heightMap[x][z] + 1 + i][z] = DataDictionary::getCube("Wood");
    }

    for (int r = 5; r > 0; --r) {
        for (int i = -r / 2; i <= r / 2; ++i) {
            for (int j = -r / 2; j <= r / 2; ++j) {
                data[x + i][(int)heightMap[x][z] + 8 + (5 - r)][z + j] = DataDictionary::getCube("Leaves");
            }
        }
    }
    modified = true;
}

int Chunk::groundLevel(const int x, const int z) const {
    return this->heightMap[x][z];
}

void Chunk::updateSurroundings() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (adyacent[i][j]) {
                adyacent[i][j]->modified = true;
            }
        }
    }
}

std::array<glm::vec3, 8> Chunk::getOffsets() {
    std::array<glm::vec3, 8> offsets;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int w = 0; w < 2; ++w) {
                int index = i * 4 + j * 2 + w;
                offsets[index] = {i * ChunkWidth, j * ChunkHeight, w * ChunkLength};
            }
        }
    }
    return offsets;
}

const Cube* Chunk::getCube(int x, int y, int z) {
    return data[x][y][z];
}

std::vector<glm::vec3> Chunk::getPoints(const std::vector<glm::vec3>& chunks) {
    std::array<glm::vec3, 8> offsets = getOffsets();

    std::vector<glm::vec3> points;
    points.reserve(8 * chunks.size());
    for (auto& a : chunks) {
        for (auto& offs : offsets) {
            points.push_back(a * dim + offs);
        }
    }
    return points;
}