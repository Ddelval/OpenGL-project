#include "Graphics/Mesh.hpp"

using namespace Graphics;

const int default_vertices = 2 * 24;
const int default_triangles = 2 * 12;

Vertex::Vertex(glm::vec2 pos) {
    position = glm::vec3(pos, 0);
    texCoord = pos;
    texId = 0;
}
Vertex::Vertex(const Vertex& v)
    : position(v.position), normal(v.normal), texCoord(v.texCoord), texId(v.texId) {
}
Vertex::Vertex(const Vertex&& v)
    : position(v.position), normal(v.normal), texCoord(v.texCoord), texId(v.texId) {
}

Vertex& Vertex::operator=(const Vertex& v) {
    this->position = v.position;
    this->texCoord = v.texCoord;
    this->normal = v.normal;
    this->texId = v.texId;
    return *this;
}

Vertex& Vertex::operator=(const Vertex&& v) {
    this->position = std::move(v.position);
    this->texCoord = std::move(v.texCoord);
    this->normal = v.normal;
    this->texId = v.texId;
    return *this;
}

void Vertex::applyTransform(const glm::vec3& offset, const glm::mat4& transform) {
    this->position = transform * glm::vec4(this->position, 1);
    this->position += offset;
    this->normal = transform * glm::vec4(this->normal, 1);
}
 void Vertex::applyOffset(const glm::vec3& offset) {
    this->position += offset;
}

Mesh::Mesh() {
    for (int i = 0; i < maxTextures; ++i) textureIndeces[i] = i;
}
Mesh::Mesh(std::vector<Vertex>&& vert, std::vector<int>&& indec)
    : vertices(std::move(vert)), indeces(std::move(indec)) {
    for (int i = 0; i < maxTextures; ++i) textureIndeces[i] = i;
    generateGLData(vertices.size(), indeces.size() / 3);
    first = false;
}

Mesh::Mesh(const Mesh& m) {
    *this = m;
    for (int i = 0; i < maxTextures; ++i) textureIndeces[i] = i;
}

Mesh::Mesh(const Mesh&& m) 
:vertices(move(m.vertices)),indeces(move(m.indeces)),textures(move(m.textures)){
    texturesUsed = m.texturesUsed;
    changed = m.changed;
    first = m.first;
    VAO = m.VAO;
    VBO = m.VBO;
    EBO = m.EBO;
    for (int i = 0; i < maxTextures; ++i) textureIndeces[i] = i;
}

void Mesh::generateGLData(int nvertices, int ntriangles) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ntriangles * 3 * sizeof(int), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoord)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texId)));
    glEnableVertexAttribArray(3);
}

Mesh& Mesh::operator=(const Mesh&& m) {
    this->vertices = move(m.vertices);
    this->indeces = move(m.indeces);
    this->textures = move(m.textures);
    this->texturesUsed = m.texturesUsed;
    this->changed = m.changed;
    this->VAO = m.VAO;
    this->VBO = m.VBO;
    this->EBO = m.EBO;
    return *this;
}

Mesh& Mesh::operator=(const Mesh& m) {
    this->vertices = m.vertices;
    this->indeces = m.indeces;
    this->textures = m.textures;
    this->texturesUsed = m.texturesUsed;
    this->changed = m.changed;
    this->VAO = m.VAO;
    this->VBO = m.VBO;
    this->EBO = m.EBO;
    return *this;
}

void Mesh::setTextures(const std::vector<const Texture*>& tex) {
    if (tex.size() > maxTextures) {
        std::cerr << "Too many textures" << std::endl;
    }
    this->textures = tex;
    this->texturesUsed = tex.size();
}
void Mesh::applyTransform(const glm::vec3& offset, const glm::mat4& transform) {
    for (auto& a : vertices) a.applyTransform(offset, transform);
}
void Mesh::applyOffset(const glm::vec3& offset) {
    for (auto& a : vertices) a.applyOffset(offset);
}

void Mesh::draw(const Shader& s) {
    if (first) {
        generateGLData(vertices.size(), indeces.size() / 3);
        first = false;
    }
    for (int i = 0; i < texturesUsed; ++i) {
        textures[i]->activateTexture(i);
        //std::cout << "tex: " << i << textures[i]->name << std::endl;
    }
    if (s.tex) s.setIntArray("textures", this->textureIndeces, maxTextures);
    glBindVertexArray(VAO);
    if (changed) {
        /*for (int i = 0; i < texturesUsed; ++i) {
            //std::cout <<textures[i]<<" "<< textures[i]->name <<","<<textures[i]->id<< " ->" << i << std::endl;
        }*/
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertices.size(), vertices.data());
        /*for (int i = 0; i < vertices.size(); ++i){
            //std::cout << "v " << vertices[i].normal.x << "," << vertices[i].normal.y << "," << vertices[i].normal.z << std::endl;
        }*/
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(int) * indeces.size(), indeces.data());
        /*for (int i = 0; i < indeces.size(); ++i) {
            //std::cout << "i " << indeces[i] << std::endl;
        }*/
        changed = false;
    }
    glDrawElements(GL_TRIANGLES, indeces.size(), GL_UNSIGNED_INT, 0);
}

const std::vector<const Texture*>& Mesh::getTextures() const {
    return textures;
}

const std::vector<int>& Mesh::getIndeces() const {
    return this->indeces;
}
const std::vector<Vertex>& Mesh::getVertices() const {
    return this->vertices;
}