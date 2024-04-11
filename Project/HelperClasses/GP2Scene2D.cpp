#include "GP2Scene2D.h"

#include "GP2CircleMesh.h"
#include "GP2RectangleMesh.h"
#include "GP2RoundedRectangleMesh.h"

GP2Scene2D::GP2Scene2D()
{
	GP2CircleMesh circleMesh{ glm::vec2{0.25f, -0.5f}, 0.35f, 0.25f, 32 };
	AddMesh(circleMesh);
	GP2RectangleMesh rectangleMesh{ glm::vec2{-0.5f, -0.5f}, 0.65f, 0.45f };
	AddMesh(rectangleMesh);
	GP2RoundedRectangleMesh roundedRectangleMesh{ glm::vec2{0.25f, 0.65f}, 0.75f, 0.5f, 0.25f };
	AddMesh(rectangleMesh);
}
