#include "GP2CubeMesh.h"
GP2CubeMesh::GP2CubeMesh(const glm::vec3& center, const float width, const float height)
{
	m_Position = center;
	//TopFace
	AddVertex({ center.x - (0.5f * width), center.y - (0.5f * height), center.z + 0.5f }, { 0.0f, 1.0f, 0.0f });
	AddVertex({ center.x + (0.5f * width), center.y - (0.5f * height), center.z + 0.5f }, { 0.0f, 1.0f, 0.0f });
	AddVertex({ center.x + (0.5f * width), center.y + (0.5f * height), center.z + 0.5f }, { 0.0f, 1.0f, 0.0f });
	AddVertex({ center.x - (0.5f * width), center.y + (0.5f * height), center.z + 0.5f }, { 0.0f, 1.0f, 0.0f });

	//t1
	AddIndex(2);
	AddIndex(1);
	AddIndex(0);

	//t2
	AddIndex(0);
	AddIndex(3);
	AddIndex(2);

	//Front face
	AddVertex({ center.x + (0.5f * width), center.y - (0.5f * height), center.z - 0.5f }, { 1.0f, 0.0f, 0.0f }, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	AddVertex({ center.x + (0.5f * width), center.y - (0.5f * height), center.z + 0.5f }, { 1.0f, 0.0f, 0.0f }, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f});
	AddVertex({ center.x + (0.5f * width), center.y + (0.5f * height), center.z - 0.5f }, { 1.0f, 0.0f, 0.0f }, {1.0f, 1.0f, 1.0f}, {1.0f, .0f});
	AddVertex({ center.x + (0.5f * width), center.y + (0.5f * height), center.z + 0.5f }, { 1.0f, 0.0f, 0.0f }, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f});

	//t1
	AddIndex(4);
	AddIndex(5);
	AddIndex(6);

	//t2
	AddIndex(7);
	AddIndex(6);
	AddIndex(5);

	//Back face
	AddVertex({ center.x - (0.5f * width), center.y - (0.5f * height), center.z - 0.5f }, { 1.0f, 0.0f, 0.0f });
	AddVertex({ center.x - (0.5f * width), center.y - (0.5f * height), center.z + 0.5f }, { 1.0f, 0.0f, 0.0f });
	AddVertex({ center.x - (0.5f * width), center.y + (0.5f * height), center.z - 0.5f }, { 1.0f, 0.0f, 0.0f });
	AddVertex({ center.x - (0.5f * width), center.y + (0.5f * height), center.z + 0.5f }, { 1.0f, 0.0f, 0.0f });

	//t1
	AddIndex(10);
	AddIndex(9);
	AddIndex(8);

	//t2
	AddIndex(9);
	AddIndex(10);
	AddIndex(11);

	//Right face
	AddVertex({ center.x - (0.5f * width), center.y + (0.5f * height), center.z - 0.5f }, { 0.0f, 0.0f, 1.0f });
	AddVertex({ center.x - (0.5f * width), center.y + (0.5f * height), center.z + 0.5f }, { 0.0f, 0.0f, 1.0f });
	AddVertex({ center.x + (0.5f * width), center.y + (0.5f * height), center.z - 0.5f }, { 0.0f, 0.0f, 1.0f });
	AddVertex({ center.x + (0.5f * width), center.y + (0.5f * height), center.z + 0.5f }, { 0.0f, 0.0f, 1.0f });

	//t1
	AddIndex(14);
	AddIndex(13);
	AddIndex(12);

	//t2
	AddIndex(13);
	AddIndex(14);
	AddIndex(15);

	//Left face
	AddVertex({ center.x - (0.5f * width), center.y - (0.5f * height), center.z - 0.5f }, { 0.0f, 0.0f, 1.0f });
	AddVertex({ center.x - (0.5f * width), center.y - (0.5f * height), center.z + 0.5f }, { 0.0f, 0.0f, 1.0f });
	AddVertex({ center.x + (0.5f * width), center.y - (0.5f * height), center.z - 0.5f }, { 0.0f, 0.0f, 1.0f });
	AddVertex({ center.x + (0.5f * width), center.y - (0.5f * height), center.z + 0.5f }, { 0.0f, 0.0f, 1.0f });

	//t1
	AddIndex(16);
	AddIndex(17);
	AddIndex(18);

	//t2
	AddIndex(19);
	AddIndex(18);
	AddIndex(17);

	//BottomFace
	AddVertex({ center.x - (0.5f * width), center.y - (0.5f * height), center.z - 0.5f }, { 0.0f, 1.0f, 0.0f });
	AddVertex({ center.x + (0.5f * width), center.y - (0.5f * height), center.z - 0.5f }, { 0.0f, 1.0f, 0.0f });
	AddVertex({ center.x + (0.5f * width), center.y + (0.5f * height), center.z - 0.5f }, { 0.0f, 1.0f, 0.0f });
	AddVertex({ center.x - (0.5f * width), center.y + (0.5f * height), center.z - 0.5f }, { 0.0f, 1.0f, 0.0f });

	//t1
	AddIndex(20);
	AddIndex(21);
	AddIndex(22);

	//t2
	AddIndex(20);
	AddIndex(22);
	AddIndex(23);
}