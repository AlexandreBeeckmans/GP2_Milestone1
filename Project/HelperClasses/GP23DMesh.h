#pragma once
#include <string>

#include "GP2Mesh.h"

class GP23DMesh final : public GP2Mesh
{
public:
	GP23DMesh();
	GP23DMesh(const std::string& filePath);

	~GP23DMesh() = default;

	GP23DMesh(const GP23DMesh& other) = delete;
	GP23DMesh(GP23DMesh&& other) noexcept = default;
	GP23DMesh& operator=(const GP23DMesh& other) = delete;
	GP23DMesh& operator=(GP23DMesh&& other) noexcept = default;

private:
	std::string m_filePath{};
};
