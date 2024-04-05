#pragma once
#include "GP2Mesh.h"

class GP2CircleMesh final : public GP2Mesh
{
public:
	GP2CircleMesh(const glm::vec2& center, const float radiusX, const float radiusY, const size_t nbOfSides);
};