#include "GP23DMesh.h"

#include "OBJLoader.h"

GP23DMesh::GP23DMesh() :
	GP2Mesh(),
	m_filePath{ "resources/vehicle.obj" }
{
	std::vector<Vertex> vertices{};
	std::vector<uint32_t> indices{};

	ParseOBJ(m_filePath, vertices, indices);



	//for the moment clamp values between -1 and 1
	float min{ FLT_MAX };
	float max{ FLT_MIN };

	for (const Vertex& vertex : vertices)
	{
		//get min x
		if(vertex.pos.x < min)
		{
			min = vertex.pos.x;
		}

		//get max x
		if (vertex.pos.x > max)
		{
			max = vertex.pos.x;
		}

		//get min y
		if (vertex.pos.x < min)
		{
			min = vertex.pos.y;
		}

		//get max Y
		if (vertex.pos.x > max)
		{
			max = vertex.pos.y;
		}

		//get min z
		if (vertex.pos.z < min)
		{
			min = vertex.pos.z;
		}

		//get max z
		if (vertex.pos.z > max)
		{
			max = vertex.pos.x;
		}
	}

	const float totalDifference{ max - min };
	for (Vertex& vertex : vertices)
	{
		//clamp all x values between -1 and 1
		const float xDifference{ max - vertex.pos.x };
		vertex.pos.x = 2 * (1 - (xDifference / totalDifference)) - 1;



		//clamp all y values between -1 and 1
		const float yDifference{ max - vertex.pos.y };
		vertex.pos.y = 2 * (1 - (yDifference / totalDifference)) - 1;

		//clamp all z values between -1 and 1
		const float zDifference{ max - vertex.pos.z };
		vertex.pos.z = 2 * (1 - (zDifference / totalDifference)) - 1;
	}
	
	
	


	int value{ 0 };

	for(const Vertex& vertex : vertices)
	{
		//AddVertex(vertex.pos, glm::vec3{ static_cast<float>(1 - value),0.5f,float(value)});
		AddVertex(vertex.pos, glm::vec3{ 1.0f,0.5f,0 });
		++value %= 3;
	}

	for(const uint32_t& index: indices)
	{
		AddIndex(static_cast<uint16_t>(index));
	}
}
