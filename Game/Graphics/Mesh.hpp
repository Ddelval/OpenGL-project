#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include "GeneralModules/Shader.hpp"
#include "GeneralModules/Texture.hpp"
#include "GeneralModules/utility.hpp"
#include "Graphics/Drawable.hpp"

namespace Graphics {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    float texId=0;

    Vertex() = default;
    Vertex(const Vertex&);
    Vertex(const Vertex&&);

    Vertex(glm::vec2 pos);
    Vertex& operator=(const Vertex&&);
    Vertex& operator=(const Vertex&);
    void applyTransform(const glm::vec3& offset, const glm::mat4& transform);
    void applyOffset(const glm::vec3& offset);
};

class Mesh {
   private:
    static constexpr int maxTextures=15;
    std::vector<Vertex> vertices;
    std::vector<int> indeces;
    std::vector<const Texture*> textures;
    //const Texture* textures[maxTextures];
    int textureIndeces[maxTextures];
    int texturesUsed = 0;
    bool changed = true;
    bool first = true;
    uint VAO;
    uint VBO;
    uint EBO;

   public:
    Mesh();
    Mesh(const Mesh &m);
    Mesh(const Mesh&& m);
    Mesh(int nvertices, int ntriangles);
    Mesh(std::vector<Vertex>&& vert, std::vector<int>&& indec);
    Mesh(std::vector<Vertex>& vert, std::vector<int> ind);

    Mesh& operator=(const Mesh&& m);
    Mesh& operator=(const Mesh& m);

    void applyTransform(const glm::vec3& offset, const glm::mat4& transform);
    void applyOffset(const glm::vec3& offset);
    void draw(const Shader& s);
    void setTextures(const std::vector<const Texture*>& textures);

    const std::vector<const Texture*>& getTextures() const;
    const std::vector<int>& getIndeces() const;
    const std::vector<Vertex>& getVertices() const;
    bool isModified() const;

   private:
    void generateGLData(int nvertices, int ntriangles);
};

}  // namespace Graphics

#endif