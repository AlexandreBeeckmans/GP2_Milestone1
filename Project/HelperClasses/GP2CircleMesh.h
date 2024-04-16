#pragma once
#include "GP2Mesh.h"

class GP2CircleMesh final : public GP2Mesh
{
public:
	GP2CircleMesh(const glm::vec2& center, float radiusX, float radiusY, size_t nbOfSegments);
	~GP2CircleMesh() = default;

	GP2CircleMesh(const GP2CircleMesh& other) = delete;
	GP2CircleMesh(GP2CircleMesh&& other) noexcept = default;
	GP2CircleMesh& operator=(const GP2CircleMesh& other) = delete;
	GP2CircleMesh& operator=(GP2CircleMesh&& other) noexcept = default;
};