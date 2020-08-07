#ifndef MAP_GAME_MODEL_HPP
#define MAP_GAME_MODEL_HPP
#include <set>
#include <vector>

#include "GameCubes/Chunk.hpp"
#include "GeneralModules/Shader.hpp"
#include "Graphics/CameraData.hpp"
#include "Graphics/Lighting/DirectionalShadowMap.hpp"
#include "Random/PerlinNoise.hpp"
namespace GameModel {

class Map : public Graphics::Drawable {
   private:
    Chunk* chunks;
    bool* created;
    int xsize;
    int ysize;
    int zsize;
    int seed;
    siv::PerlinNoise noise;
    Lighting::DirectionalShadowMap close;
    Lighting::DirectionalShadowMap far;
    Shader shadowShader;
    Shader dataShader;

    std::vector<glm::vec3> seenChunks;
    std::vector<glm::vec3> closeChunks;

    Chunk& getChunk(int x, int y, int z);
    Chunk* safeGetChunk(int x, int y, int z);
    void generateWorld();

   public:
    Map() = default;
    Map(int xsize, int ysize, int zsize, int seed, Shader shadow, Shader data);
    bool checkEmtpySpace(float x, float y, float z);
    void addTree(const int x, const int y);

    void draw(Shader& s, Graphics::CameraData& cam);
    void draw(Shader& s);
    void draw(Shader& s, std::vector<glm::vec3>& chunks);
    void setupShadows(Graphics::CameraData& cam);
    std::vector<glm::vec3> visibleChunksCoordinates(const Graphics::CameraData& c);
    glm::vec3 getFirstBlock(glm::vec3 pos, glm::vec3 dir, float distMax);
    void setBlock(int x, int y, int z, GameModel::Cube* b);
    void updateBlock(int x, int y, int z, GameModel::Cube* b);

   private:
    bool testChunkVisible(std::set<int>& v, int chunkX, int chunkZ, const Graphics::CameraData& cam);
    void visibleChunksDfs(std::set<int>& v, int chunkX, int chunkZ, const Graphics::CameraData& cam);
    void generateChunk(const int x, const int y, const int z);
    std::vector<int> visibleChunks(const Graphics::CameraData& c);
    int position(int x, int y, int z);
    int position(glm::vec3 p);
    const Cube* getCube(int x, int y, int z);
    
};
}  // namespace GameModel

#endif
