#pragma once
#include "GP2Mesh.h"

class GP2CubeMesh final : public GP2Mesh
{
public:
	GP2CubeMesh(const glm::vec2& center, const float width, const float height);
};