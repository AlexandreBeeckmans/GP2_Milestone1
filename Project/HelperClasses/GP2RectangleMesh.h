#pragma once
#include "GP2Mesh.h"


class GP2RectangleMesh : public GP2Mesh
{
public:
	GP2RectangleMesh(const glm::vec2& center, const float width, const float height);

protected:
	void InitRectangleVertices(const glm::vec2& center, const float width, const float height, const int baseIndex = 0);
};
