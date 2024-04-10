#include "GP2RectangleMesh.h"
#include "Vertex.h"


GP2RectangleMesh::GP2RectangleMesh(const glm::vec2& center, const float width, const float height)
{
	InitRectangleVertices(center, width, height);
}

void GP2RectangleMesh::InitRectangleVertices(const glm::vec2& center, const float width, const float height, const int baseIndex)
{
	////Triangle 1
	//AddVertex(glm::vec2{ center.x - width / 2.0f, center.y - height / 2.0f }, { 156 / 255.0f, 236 / 255.0f, 251 / 255.0f });
	//AddVertex(glm::vec2{ center.x + width / 2.0f, center.y - height / 2.0f }, { 0.0f, 1.0f, 0.0f });
	//AddVertex(glm::vec2{ center.x - width / 2.0f, center.y + height / 2.0f }, { 0.0f, 0.0f, 1.0f });

	////Triangle 2
	//AddVertex(glm::vec2{ center.x + width / 2.0f, center.y + height / 2.0f }, { 1.0f, 1.0f, 28 / 255.0f });

	//AddIndex(baseIndex);
	//AddIndex(baseIndex + 1);
	//AddIndex(baseIndex + 2);

	//AddIndex(baseIndex + 2);
	//AddIndex(baseIndex + 1);
	//AddIndex(baseIndex + 3);



	AddVertex({-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f});
	AddVertex({0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f});
	AddVertex({0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f});
	AddVertex({-0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f});

	AddVertex({-0.255f, -0.75f, -0.5f}, {0.0f, 0.0f, 1.0f});
	AddVertex({0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f});
	AddVertex({0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f});
	AddVertex({-0.35f, 0.35f, -0.5f}, {0.0f, 1.0f, 0.0f});

	AddIndex(baseIndex + 0);
	AddIndex(baseIndex + 1);
	AddIndex(baseIndex + 2);
	AddIndex(baseIndex + 2);
	AddIndex(baseIndex + 3);
	AddIndex(baseIndex + 0);

	AddIndex(baseIndex + 4);
	AddIndex(baseIndex + 7);
	AddIndex(baseIndex + 0);


	AddIndex(baseIndex + 4);
	AddIndex(baseIndex + 5);
	AddIndex(baseIndex + 6);
	AddIndex(baseIndex + 6);
	AddIndex(baseIndex + 7);
	AddIndex(baseIndex + 4);
}