#include "GP2CircleMesh.h"
#include "Vertex.h"

GP2CircleMesh::GP2CircleMesh(const glm::vec2& center, const float radiusX, const float radiusY, const size_t nbOfSegments) :
	GP2Mesh::GP2Mesh()
{
	AddVertex({ center.x, center.y }, { 1.0f, 0.0f, 0.0f });
	//AddIndex(0);
	for (size_t i{ 0 }; i < nbOfSegments; ++i)
	{
		const float pi{ 3.1416f };

		const float angle{ (2 * pi) / nbOfSegments * i };
		const float nextAngle{ (2 * pi) / nbOfSegments * (i + 1) };

		
		AddVertex({ center.x + std::cosf(angle) * radiusX, center.y + std::sinf(angle) * radiusY }, { 0.0f, std::cosf(angle), std::sinf(angle) });
	}


	//Add the indexes
	for (int i{ 0 }; i < nbOfSegments; ++i)
	{
		AddIndex(0);
		AddIndex(i+1);
		AddIndex(((i+1)%(nbOfSegments))+1);
	}
}