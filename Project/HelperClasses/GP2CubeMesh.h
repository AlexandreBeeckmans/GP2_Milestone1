#pragma once
#include "GP2Mesh.h"

class GP2CubeMesh final : public GP2Mesh
{
public:
	GP2CubeMesh(const glm::vec3& center, const float width, const float height);
	~GP2CubeMesh() = default;

	GP2CubeMesh(const GP2CubeMesh& other) = delete;
	GP2CubeMesh(GP2CubeMesh&& other) noexcept = default;
	GP2CubeMesh& operator=(const GP2CubeMesh& other) = delete;
	GP2CubeMesh& operator=(GP2CubeMesh&& other) noexcept = default;
};