#include "GP23DMesh.h"

#include <random>

#include "OBJLoader.h"

GP23DMesh::GP23DMesh() :
	GP2Mesh(),
	m_filePath{ "resources/vehicle.obj" }
{
	std::vector<Vertex> vertices{};
	std::vector<uint32_t> indices{};

	ParseOBJ(m_filePath, vertices, indices);

	//Temporary, just to get some result
	float minX{FLT_MAX};
	float maxX{FLT_MIN};
	float minY{ FLT_MAX };
	float maxY{ FLT_MIN };
	float minZ{ FLT_MAX };
	float maxZ{ FLT_MIN };

	for (const Vertex& vertex : vertices)
	{
		//get min x
		if(vertex.pos.x < minX)
		{
			minX = vertex.pos.x;
		}

		//get max x
		if (vertex.pos.x > maxX)
		{
			maxX = vertex.pos.x;
		}

		//get min y
		if (vertex.pos.x < minY)
		{
			minY = vertex.pos.y;
		}

		//get max Y
		if (vertex.pos.x > maxY)
		{
			maxY = vertex.pos.y;
		}

		//get min z
		if (vertex.pos.z < minZ)
		{
			minZ = vertex.pos.z;
		}

		//get max z
		if (vertex.pos.z > maxZ)
		{
			maxZ = vertex.pos.x;
		}
	}

	const float xTotalDifference{ maxX - minX };
	const float yTotalDifference{ maxY - minY };
	const float zTotalDifference{ maxZ - minZ };
	for (Vertex& vertex : vertices)
	{
		//clamp all x values between -1 and 1
		const float xDifference{ maxX - vertex.pos.x };
		vertex.pos.x = 2 * (1 - (xDifference / xTotalDifference)) - 1;



		//clamp all y values between -1 and 1
		const float yDifference{ maxY - vertex.pos.y };
		vertex.pos.y = 2 * (1 - (yDifference / yTotalDifference)) - 1;

		//clamp all z values between -1 and 1
		const float zDifference{ maxZ - vertex.pos.z };
		vertex.pos.z = 2 * (1 - (zDifference / zTotalDifference)) - 1;
	}
	
	
	

	std::default_random_engine gen;
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

	for(const Vertex& vertex : vertices)
	{
		const float randR{ distribution(gen) };
		const float randG{ distribution(gen) };
		const float randB{ distribution(gen) };
		AddVertex(vertex.pos, glm::vec3{ randR,randG,randB });
	}

	for(const uint32_t& index: indices)
	{
		AddIndex(static_cast<uint16_t>(index));
	}

	//add index in reverse order
	/*for(int i{0}; i < indices.size(); ++i)
	{
		AddIndex(static_cast<uint16_t>(indices[indices.size() - 1 - i]));
	}*/
}
