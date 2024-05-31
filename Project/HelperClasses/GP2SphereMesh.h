#pragma once
#include "GP2Mesh.h"

class GP2SphereMesh final : public GP2Mesh
{
public:
	GP2SphereMesh(const glm::vec3& center, const float radius);
	~GP2SphereMesh() = default;

	GP2SphereMesh(const GP2SphereMesh& other) = delete;
	GP2SphereMesh(GP2SphereMesh&& other) noexcept = default;
	GP2SphereMesh& operator=(const GP2SphereMesh& other) = delete;
	GP2SphereMesh& operator=(GP2SphereMesh&& other) noexcept = default;
};
