#ifndef QUAD_GRAPHICS_HPP
#define QUAD_GRAPHICS_HPP
#include "Graphics/Mesh.hpp"
#include <array>

namespace Graphics::MeshBuilder {

Mesh buildQuad(const glm::vec3& offset, const glm::mat4& transform, const Texture* texture);
std::array<Mesh,6> buildCube(const glm::vec3& offset, const glm::mat4& transform, const std::array<const Texture*, 6> textures);

Mesh getOutterCube(const glm::vec3& offsets, const std::array<const Texture*, 6> textures);

Mesh joinMesh(const Mesh* m1, const Mesh* m2);
Mesh joinMesh(const std::vector<const Mesh*> meshes);
Mesh joinMesh(const std::vector<const Mesh*>& meshes, const std::vector<glm::vec3>* offsets);

}  // namespace Graphics::MeshBuilder

#endif