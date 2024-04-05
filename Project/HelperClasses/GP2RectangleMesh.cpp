#include "GP2RectangleMesh.h"
#include "Vertex.h"


GP2RectangleMesh::GP2RectangleMesh(const glm::vec2& center, const float width, const float height)
{
	InitRectangleVertices(center, width, height);
}

void GP2RectangleMesh::InitRectangleVertices(const glm::vec2& center, const float width, const float height)
{
	//Triangle 1
	AddVertex(glm::vec2{ center.x - width / 2.0f, center.y - height / 2.0f }, { 156 / 255.0f, 236 / 255.0f, 251 / 255.0f });
	AddVertex(glm::vec2{ center.x + width / 2.0f, center.y - height / 2.0f }, { 0.0f, 1.0f, 0.0f });
	AddVertex(glm::vec2{ center.x - width / 2.0f, center.y + height / 2.0f }, { 0.0f, 0.0f, 1.0f });

	//Triangle 2
	//AddVertex(glm::vec2{ center.x - width / 2.0f, center.y + height / 2.0f }, { 0.0f, 0.0f, 1.0f });
	//AddVertex(glm::vec2{ center.x + width / 2.0f, center.y - height / 2.0f }, { 0.0f, 1.0f, 0.0f });
	AddVertex(glm::vec2{ center.x + width / 2.0f, center.y + height / 2.0f }, { 1.0f, 1.0f, 28 / 255.0f });

	AddIndex(0);
	AddIndex(1);
	AddIndex(2);

	AddIndex(2);
	AddIndex(1);
	AddIndex(3);
}