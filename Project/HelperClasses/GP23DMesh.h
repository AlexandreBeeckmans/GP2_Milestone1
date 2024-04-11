#pragma once
#include <string>

#include "GP2Mesh.h"

class GP23DMesh final : public GP2Mesh
{
public:
	GP23DMesh();

private:
	std::string m_filePath{};
};
