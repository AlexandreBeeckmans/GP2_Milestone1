#include "GP23DMesh.h"

#include "OBJLoader.h"

GP23DMesh::GP23DMesh(const glm::vec3& position) :
	GP23DMesh("resources/vehicle.obj", position)
{}


GP23DMesh::GP23DMesh(const std::string& filePath, const glm::vec3& position, const float scale):
	GP2Mesh(position),
	m_filePath{ filePath }
{
	std::vector<Vertex3D> vertices{};
	std::vector<uint32_t> indices{};

	ParseOBJ(m_filePath, vertices, indices);

	for(const Vertex3D& vertex : vertices)
	{
		Vertex3D vertexToAdd{};
		vertexToAdd.pos = scale * vertex.pos + position;
		vertexToAdd.color = glm::vec3{ 1.0f,0.5f,0 };
		vertexToAdd.normal = vertex.normal;
		vertexToAdd.uv = vertex.uv;
		vertexToAdd.tangent = vertex.tangent;
		AddVertex(vertexToAdd);
	}

	for(const uint32_t& index: indices)
	{
		AddIndex(static_cast<uint16_t>(index));
	}
}
