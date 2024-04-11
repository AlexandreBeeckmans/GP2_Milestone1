#include "GP2Scene3D.h"

#include "GP23DMesh.h"

GP2Scene3D::GP2Scene3D()
{
	/*GP2CubeMesh cubeMesh{ glm::vec2{-0.5f, -0.5f}, 0.65f, 0.45f };
	m_Meshes.push_back(std::move(cubeMesh));*/
	GP23DMesh loadedMesh{};
	AddMesh(loadedMesh);
}
