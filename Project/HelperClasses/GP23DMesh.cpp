#include "GP23DMesh.h"

#include "OBJLoader.h"

GP23DMesh::GP23DMesh() :
	GP23DMesh("resources/vehicle.obj")
{}


GP23DMesh::GP23DMesh(const std::string& filePath):
	GP2Mesh(),
	m_filePath{ filePath }
{
	std::vector<Vertex> vertices{};
	std::vector<uint32_t> indices{};

	ParseOBJ(m_filePath, vertices, indices);

	for(const Vertex& vertex : vertices)
	{
		Vertex vertexToAdd{};
		vertexToAdd.pos = vertex.pos;
		vertexToAdd.color = glm::vec3{ 1.0f,0.5f,0 };
		vertexToAdd.normal = vertex.normal;
		AddVertex(vertexToAdd);
	}

	for(const uint32_t& index: indices)
	{
		AddIndex(static_cast<uint16_t>(index));
	}
}
