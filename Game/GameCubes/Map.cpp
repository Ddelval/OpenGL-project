#include "GameCubes/Map.hpp"

#include <functional>
#include <iomanip>
#include <set>

#include "GameCubes/DataDictionary.hpp"
#include "GameCubes/Objects/Placeable.hpp"
using namespace GameModel;
const int maxX = 100;
const int maxZ = 100;

Map::Map(int xsize, int ysize, int zsize, int seed, Shader shadow, Shader data)
    : xsize(xsize), ysize(ysize), zsize(zsize), seed(seed), noise((std::uint32_t)seed), shadowShader(shadow), dataShader(data), close(0, 0, shadow), far(0, 0, shadow) {
    int siz = xsize * ysize * zsize;
    chunks = new Chunk[siz];
    created = new bool[siz];
    memset(created, 0, siz);

    close.setDirections(glm::vec3(50, 40, -20), glm::vec3(80, 0, 80), 10, 10);
    close.setDrawable(this);

    far.setDirections(glm::vec3(50, 40, -20), glm::vec3(80, 0, 80), 10, 10);
    far.setDrawable(this);
}
Chunk& Map::getChunk(int x, int y, int z) {
    generateChunk(x, y, z);
    return this->chunks[x * (ysize * zsize) + y * zsize + z];
}

inline int Map::position(int x, int y, int z) {
    return z + y * zsize + x * ysize * zsize;
}
inline int Map::position(glm::vec3 p) {
    return p.z + p.y * zsize + p.x * ysize * zsize;
}
Chunk* Map::safeGetChunk(int x, int y, int z) {
    if (x < 0 || x >= xsize) return nullptr;
    if (y < 0 || y >= ysize) return nullptr;
    if (z < 0 || z >= zsize) return nullptr;

    return &this->chunks[x * (ysize * zsize) + y * zsize + z];
}
void Map::generateChunk(const int x, const int y, const int z) {
    Chunk& cc = this->chunks[x * (ysize * zsize) + y * zsize + z];
    if (created[x * (ysize * zsize) + y * zsize + z] == false) {
        created[x * (ysize * zsize) + y * zsize + z] = true;
        cc.setPosition(glm::vec3(x * Chunk::ChunkWidth, y * Chunk::ChunkHeight, z * Chunk::ChunkLength));
        for (int x2 = x * 16; x2 < (x + 1) * 16; ++x2) {
            for (int z2 = z * 16; z2 < (z + 1) * 16; ++z2) {
                int h = (int)(noise.accumulatedOctaveNoise2D_0_1(x2 / double(maxX), z2 / double(maxZ), 40) * Chunk::ChunkHeight * ysize);
                int lim = std::min(Chunk::ChunkHeight, h - Chunk::ChunkHeight * y);
                cc.setHeight(x2 % 16, z2 % 16, lim);
                for (int i = 0; i < lim; ++i) {
                    const Cube* c;
                    if (i + Chunk::ChunkHeight * y < 10) {
                        c = DataDictionary::getCube("Grass");
                    }

                    else if (i + Chunk::ChunkHeight * y < 20) {
                        c = DataDictionary::getCube("Rock");
                    }

                    else {
                        c = DataDictionary::getCube("Snow");
                    }

                    cc.setCube(x2 % 16, i, z2 % 16, c);
                }
            }
        }
        cc.setSurroundings(safeGetChunk(x - 1, y, z), safeGetChunk(x + 1, y, z),
                           safeGetChunk(x, y - 1, z), safeGetChunk(x, y + 1, z),
                           safeGetChunk(x, y, z - 1), safeGetChunk(x, y, z + 1));
        cc.updateSurroundings();
    }
}

void Map::draw(Shader& s) {
    for (int i = 0; i < xsize; ++i) {
        for (int j = 0; j < ysize; ++j) {
            for (int w = 0; w < zsize; ++w) {
                getChunk(i, j, w).draw(s);
            }
        }
    }
}
void Map::draw(Shader& s, std::vector<glm::vec3>& chunkPos) {
    //std::cout << "Drawing " << chunkPos.size() << std::endl;
    for (auto a : chunkPos) {
        //std::cout << position(a) << " ";
        chunks[position(a)].draw(s);
    }
}

void Map::addTree(const int x0, const int z0) {
    int y0 = getChunk(x0 / 16, 0, z0 / 16).groundLevel(x0 % 16, z0 % 16);
    Placeable pca;
    pca.drawTree();

    auto f = [&](int x, int y, int z, const Cube* c) {
        if (c == nullptr) return;
        x += x0;
        y += y0;
        z += z0;
        if (x < 0 || y < 0 || z < 0) return;
        getChunk(x / 16, 0, z / 16).setCube((x + 16) % 16, (y + 32) % 32, (z + 16) % 16, c);
    };
    pca.forEach(f);
}

bool Map::testChunkVisible(std::set<int>& v, int chunkX, int chunkZ, const Graphics::CameraData& cam) {
    if (chunkX < 0 || chunkX >= this->xsize) return false;
    if (chunkZ < 0 || chunkZ >= this->zsize) return false;
    if (v.count(chunkX * ysize * zsize + chunkZ)) return false;
    bool close = false;
    bool angle = false;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            glm::vec2 p = glm::vec2(16 * (chunkX + i), 16 * (chunkZ + j));
            glm::vec2 pos = p - cam.pos;
            float distance = glm::length(pos);
            if (distance <= cam.drawingDistance) close = true;
            if (glm::dot(pos, cam.dir) >= distance * cam.hcos) angle = true;
            if (close && angle) return true;
        }
    }
    return false;
}
int counter = 0;
void Map::visibleChunksDfs(std::set<int>& v, int chunkX, int chunkZ, const Graphics::CameraData& cam) {
    v.insert(chunkX * ysize * zsize + chunkZ);
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (testChunkVisible(v, chunkX + i, chunkZ + j, cam)) {
                visibleChunksDfs(v, chunkX + i, chunkZ + j, cam);
            }
        }
    }
}

std::vector<int> Map::visibleChunks(const Graphics::CameraData& c) {
    int currentChunkX = c.pos.x / 16;
    int currentChunkZ = c.pos.y / 16;
    std::set<int> vis;
    visibleChunksDfs(vis, currentChunkX, currentChunkZ, c);
    const int adyChunks[8][2] = {
        {0, 1}, {0, -1}, {-1, 0}, {-1, 1}, {-1, -1}, {1, 1}, {1, 0}, {1, -1}};
    for (int i = 0; i < 8; ++i) {
        if (currentChunkX + adyChunks[i][0] < 0 || currentChunkX + adyChunks[i][0] >= xsize) continue;
        if (currentChunkZ + adyChunks[i][1] < 0 || currentChunkZ + adyChunks[i][1] >= zsize) continue;
        vis.insert(position(currentChunkX + adyChunks[i][0], 0, currentChunkZ + adyChunks[i][1]));
    }

    return std::vector<int>(vis.begin(), vis.end());
}

std::vector<glm::vec3> Map::visibleChunksCoordinates(const Graphics::CameraData& c) {
    auto t = visibleChunks(c);
    std::vector<glm::vec3> vec;
    vec.reserve(t.size());

    int m2 = ysize * zsize;
    for (auto a : t) {
        vec.push_back(glm::vec3(a / m2, (a / zsize) % ysize, a % zsize));
    }
    return vec;
}
void renderQuad();

void Map::draw(Shader& s, Graphics::CameraData& c) {
    std::vector<glm::vec3> seenChunks = visibleChunksCoordinates(c);
    for (auto a : seenChunks) {
        generateChunk(a.x, a.y, a.z);
    }

    std::vector<glm::vec3> seenPoints = Chunk::getPoints(seenChunks);
    auto fun = [&]() {
        this->draw(s, seenChunks);
    };
    far.generateMap(c, seenPoints, fun);

    Graphics::CameraData c2 = c;
    c2.drawingDistance = 50;

    std::vector<glm::vec3> closeChunks = visibleChunksCoordinates(c2);
    std::vector<glm::vec3> closePoints = Chunk::getPoints(closeChunks);
    auto fun2 = [&]() {
        this->draw(s, closeChunks);
    };
    close.generateMap(c2, closePoints, fun2);

    std::set<int> sset;
    for (auto a : closeChunks) {
        sset.insert(position(a));
    }

    s.use();
    int wid = 800, hei = 600;
#ifdef _APPLE_
    wid *= 2;
    hei *= 2;
#endif

    glViewport(0, 0, wid, hei);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(0.0f, 0.3f, 0.7f, 1.0f);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    for (auto a : seenChunks) {
        int pos = position(a);
        if (sset.count(pos)) {
            close.activateMap(s);
        } else {
            far.activateMap(s);
        }

        chunks[pos].draw(s);
    }

    /*Shader sdebug = Shader("shaders/debug_vertex.glsl", "shaders/debug_fragment.glsl");
    sdebug.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, close.getTexture());
    renderQuad();*/
}

bool Map::checkEmtpySpace(float x, float y, float z) {
    if (x < 0 || y < 0 || z < 0) return false;
    if (x > xsize * 16 || y > ysize * 32 || z > zsize * 16) return false;
    float cx = x - int(x / 16) * 16;
    float cy = y - int(y / 32) * 32;
    float cz = z - int(z / 16) * 16;
    return getChunk(int(x / 16), int(y / 32), int(z / 16)).freePosition(cx, cy, cz);
}

glm::vec3 Map::getFirstBlock(glm::vec3 pos, glm::vec3 dir, float distMax) {
    dir = glm::normalize(dir);
    glm::vec3 orig = pos;

    float lengths[3];
    //std::cout << "pos:" << pos << std::endl;
    //std::cout << "dir:" << dir << std::endl;
    glm::vec3 delta;
    delta.x = -1 * (dir.x < 0);
    delta.y = -1 * (dir.y < 0);
    delta.z = -1 * (dir.z < 0);
    for (int cnt = 0; cnt < 100; cnt++) {
        glm::vec3 delta2;
        delta2.x = delta.x * (pos.x == int(pos.x));
        delta2.y = delta.y * (pos.y == int(pos.y));
        delta2.z = delta.z * (pos.z == int(pos.z));
        glm::vec3 tmp = pos + delta2;
        //std::cout << "tmp " << tmp.x << " " << tmp.y << " " << tmp.z << "\n";

        if (tmp.x < 0 || tmp.y < 0 || tmp.z < 0 || glm::distance(tmp, orig) > distMax) {
            return glm::vec3{-1.0f, -1.0f, -1.0f};
        }
        if (getCube(tmp.x, tmp.y, tmp.z) != nullptr) {
            return tmp;
        }
        if (dir.x >= 0)
            lengths[0] = (1 - (pos.x - int(pos.x))) / dir.x;
        else {
            if (int(pos.x) == pos.x)
                lengths[0] = -1 / dir.x;
            else
                lengths[0] = (int(pos.x) - pos.x) / dir.x;
        }

        if (dir.y >= 0)
            lengths[1] = (1 - (pos.y - int(pos.y))) / dir.y;
        else {
            if (int(pos.y) == pos.y)
                lengths[1] = -1 / dir.y;
            else
                lengths[1] = (int(pos.y) - pos.y) / dir.y;
        }

        if (dir.z >= 0)
            lengths[2] = (1 - (pos.z - int(pos.z))) / dir.z;
        else {
            if (int(pos.z) == pos.z)
                lengths[2] = -1 / dir.z;
            else
                lengths[2] = (int(pos.z) - pos.z) / dir.z;
        }

        int mi = std::min_element(lengths, lengths + 3) - lengths;

        pos += lengths[mi] * dir;
    }
    return {-1, -1, -1};
}

void Map::updateBlock(int x, int y, int z, GameModel::Cube* b) {
    chunks[position(x / Chunk::ChunkWidth, y / Chunk::ChunkHeight, z / Chunk::ChunkLength)].updateCube(x % Chunk::ChunkWidth, y % Chunk::ChunkHeight, z % Chunk::ChunkLength, b);
}

void Map::setBlock(int x, int y, int z, GameModel::Cube* b) {
    chunks[position(x / Chunk::ChunkWidth, y / Chunk::ChunkHeight, z / Chunk::ChunkLength)].setCube(x % Chunk::ChunkWidth, y % Chunk::ChunkHeight, z % Chunk::ChunkLength, b);
}
const Cube* Map::getCube(int x, int y, int z) {
    return chunks[position(x / Chunk::ChunkWidth, y / Chunk::ChunkHeight, z / Chunk::ChunkLength)].getCube(x % Chunk::ChunkWidth, y % Chunk::ChunkHeight, z % Chunk::ChunkLength);
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad() {
    if (quadVAO == 0) {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            -1.0f,
            -1.0f,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            0.0f,
            -1.0f,
            0.0f,
            1.0f,
            0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}