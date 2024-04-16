#pragma once
#include "GP2Mesh.h"


class GP2RectangleMesh : public GP2Mesh
{
public:
	GP2RectangleMesh(const glm::vec2& center, const float width, const float height);
	~GP2RectangleMesh() = default;

	GP2RectangleMesh(const GP2RectangleMesh& other) = delete;
	GP2RectangleMesh(GP2RectangleMesh&& other) noexcept = delete;
	GP2RectangleMesh& operator=(const GP2RectangleMesh& other) = delete;
	GP2RectangleMesh& operator=(GP2RectangleMesh&& other) noexcept = delete;

protected:
	void InitRectangleVertices(const glm::vec2& center, const float width, const float height, const int baseIndex = 0);
};
