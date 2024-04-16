#include "GP2CircleMesh.h"
#include "Vertex.h"

GP2CircleMesh::GP2CircleMesh(const glm::vec2& center, float radiusX, float radiusY, size_t nbOfSegments) :
	GP2Mesh()
{
	AddVertex({ center.x, center.y }, { 1.0f, 0.0f, 0.0f });

	for (size_t i{ 0 }; i < nbOfSegments; ++i)
	{
		constexpr float pi{ 3.1416f };
		const float angle{ (2 * pi) / static_cast<float>(nbOfSegments) * i };
		AddVertex({ center.x + std::cosf(angle) * radiusX, center.y + std::sinf(angle) * radiusY }, { 0.0f, std::cosf(angle), std::sinf(angle) });
	}


	//Add the indexes
	for (size_t i{ 0 }; i < nbOfSegments; ++i)
	{
		AddIndex(0);
		AddIndex(i + 1);
		AddIndex(((i+1)%(nbOfSegments)) + 1);
	}
}