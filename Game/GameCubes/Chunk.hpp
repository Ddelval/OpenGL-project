#ifndef CHUNK_MOEL_HPP
#define CHUNK_MOEL_HPP
#include "GameCubes/Cube.hpp"
#include "Graphics/Mesh.hpp"

namespace GameModel {
class Chunk {
   public:
    static constexpr int ChunkWidth = 16;
    static constexpr int ChunkLength = 16;
    static constexpr int ChunkHeight = 32;
    static constexpr int dimensions[3] = {ChunkWidth, ChunkHeight, ChunkLength};
    inline static const glm::vec3 dim = {ChunkWidth, ChunkHeight, ChunkLength};

   private:
    glm::vec3 chunkPos = {0, 0, 0};
    const Cube* data[ChunkWidth][ChunkHeight][ChunkLength];
    float heightMap[ChunkWidth][ChunkLength];
    Graphics::Mesh processedChunk;
    bool modified = true;
    Chunk* adyacent[3][2];

   public:
    Chunk();

    const Graphics::Mesh& getMesh() const;
    void draw(const Shader& s);
    void setCube(int x, int y, int z, const Cube* cub);
    void setSurroundings(Chunk* Xprev, Chunk* Xnext, Chunk* Yprev, Chunk* Ynext, Chunk* Zprev, Chunk* Znext);
    void setHeight(int x, int z, int h);
    void setPosition(const glm::vec3& chunkPos);
    bool freePosition(float x, float y, float z);
    void addTree(const int x, const int y);
    int groundLevel(const int x, const int z) const;
    void updateSurroundings();

    const Cube* getCube(int x, int y, int z);
    void updateCube(int x, int y, int z,const Cube* cub);

    static std::vector<glm::vec3> getPoints(const std::vector<glm::vec3>& chunks);


   private:
    static std::array<glm::vec3, 8> getOffsets();
    void updateMesh();
};
}  // namespace GameModel

#endif