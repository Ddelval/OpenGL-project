#include "MeshBuilder.hpp"

#include <map>

using namespace Graphics;

const float QuadPositions[4][2] = {
    {0, 0},
    {1, 0},
    {1, 1},
    {0, 1}};
const int RenderingOrder[6] = {0, 1, 2, 0, 2, 3};

Mesh MeshBuilder::buildQuad(const glm::vec3& offset, const glm::mat4& transform, const Texture* tex) {
    std::vector<Vertex> vertices(4);
    for (int i = 0; i < 4; ++i) {
        vertices[i] = Vertex(glm::vec2(QuadPositions[i][0], QuadPositions[i][1]));
        vertices[i].normal = glm::vec3(0, 0, 1);
    }
    std::vector<int> indeces(6);
    for (int i = 0; i < 6; ++i) indeces[i] = RenderingOrder[i];

    Mesh result = Mesh(std::move(vertices), std::move(indeces));
    std::vector<const Texture*> t = {tex};
    result.setTextures(t);
    result.applyTransform(offset, transform);
    return result;
}

//Transformations for each face
const glm::mat4 rotationMatrix[] = {
    glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0)),
    glm::rotate(glm::mat4(1.0f), glm::radians(-270.0f), glm::vec3(1, 0, 0)),

    glm::rotate(glm::mat4(1.0f), glm::radians(-180.0f), glm::vec3(0, 1, 0)),
    glm::mat4(1),
    glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0, 1, 0)),
    glm::rotate(glm::mat4(1.0f), glm::radians(-270.0f), glm::vec3(0, 1, 0))};

const glm::vec3 CubeOffsets[] = {
    glm::vec3(0, 1, 1),
    glm::vec3(0, 0, 0),
    glm::vec3(1, 0, 0),
    glm::vec3(0, 0, 1),
    glm::vec3(0, 0, 0),
    glm::vec3(1, 0, 1)};

std::array<Mesh, 6> MeshBuilder::buildCube(const glm::vec3& offset, const glm::mat4& transform, const std::array<const Texture*, 6> textures) {
    std::array<Mesh, 6> faces;
    for (int i = 0; i < 6; ++i) {
        faces[i] = buildQuad(CubeOffsets[i], rotationMatrix[i], textures[i]);
        faces[i].applyTransform(offset, transform);
    }
    return faces;
}

Mesh MeshBuilder::getOutterCube(const glm::vec3& offset, const std::array<const Texture*, 6> textures) {
    glm::mat4 multi = glm::mat4(1.0f) * 1.01f;
    std::array<Mesh, 6> arr = MeshBuilder::buildCube(glm::vec3(0, 0, 0), multi, textures);
    glm::vec3 offs = offset - glm::vec3(0.005, 0.005, 0.005);
    for (auto& a : arr) a.applyOffset(offs);
    std::vector<const Mesh*> vm(6);

    for (int i = 0; i < arr.size(); ++i) {
        vm[i] = &arr[i];
    }
    return MeshBuilder::joinMesh(vm, nullptr);
}

Mesh MeshBuilder::joinMesh(const Mesh* m1, const Mesh* m2) {
    std::vector<const Texture*> textures = m1->getTextures();
    std::vector<Vertex> vertices = m1->getVertices();
    std::vector<int> indeces = m1->getIndeces();
    for (int i = 0; i < indeces.size(); ++i) std::cout << indeces[i] << " \n";
    int indexoffset = vertices.size();
    int textureOffset = textures.size();
    std::cout << " " << textureOffset << std::endl;
    for (Vertex v : m2->getVertices()) {
        v.texId += textureOffset;
        std::cout << v.texId << std::endl;
        vertices.push_back(v);
    }
    for (int i : m2->getIndeces()) indeces.push_back(i + indexoffset);
    for (auto a : m2->getTextures()) textures.push_back(a);
    for (auto a : textures) std::cout << a->name << "\n";
    Mesh m = Mesh(move(vertices), move(indeces));
    m.setTextures(textures);
    return m;
}

Mesh MeshBuilder::joinMesh(const std::vector<const Mesh*>& meshes, const std::vector<glm::vec3>* offsets = nullptr) {
    std::map<const Texture*, int> textMap;

    std::vector<const Texture*> textures;
    std::vector<Vertex> vertices;
    std::vector<int> indeces;
    int indOffset;

    int nvertices = 0;
    int nindeces = 0;
    for (auto a : meshes) {
        nvertices += a->getVertices().size();
        nindeces += a->getIndeces().size();
    }
    vertices.reserve(nvertices);
    indeces.reserve(nindeces);

    const Mesh* f = meshes[0];

    for (const Texture* t : f->getTextures()) {
        textures.push_back(t);
        textMap[t] = textures.size() - 1;
    }

    if (offsets == nullptr) {
        vertices = f->getVertices();
    } else {
        for (Vertex v : f->getVertices()) {
            v.applyOffset(offsets->operator[](0));
            vertices.push_back(v);
        }
    }
    indeces = f->getIndeces();

    int texind[64];
    for (int i = 1; i < meshes.size(); ++i) {
        const std::vector<const Texture*>& currentTex = meshes[i]->getTextures();
        for (int j = 0; j < currentTex.size(); ++j) {
            auto it = textMap.find(currentTex[j]);

            if (it != textMap.end()) {
                texind[j] = it->second;
            } else {
                textures.push_back(currentTex[j]);
                textMap[currentTex[j]] = textures.size() - 1;
                texind[j] = textures.size() - 1;
            }
        }

        indOffset = vertices.size();
        //vertices.reserve(vertices.size() + meshes[i]->getVertices().size());

        for (Vertex v : meshes[i]->getVertices()) {
            v.texId = texind[(int)v.texId];
            if (offsets != nullptr) v.applyOffset(offsets->operator[](i));
            vertices.push_back(std::move(v));
        }

        //indeces.reserve(indeces.size() + meshes[i]->getIndeces().size());
        for (const int& a : meshes[i]->getIndeces()) {
            indeces.push_back(a + indOffset);
        }
    }

    Mesh m = Mesh(move(vertices), move(indeces));
    m.setTextures(textures);

    return m;
}
