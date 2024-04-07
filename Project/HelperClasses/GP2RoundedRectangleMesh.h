#pragma once
#include "GP2RectangleMesh.h"

class GP2RoundedRectangleMesh final : public GP2RectangleMesh
{
public:
	GP2RoundedRectangleMesh(const glm::vec2& center, const float width, const float height, const float roundedPercentage);

private:
	void CalculateCorner(const glm::vec2& center, const float startingAngle, const int baseIndex);

	const float m_CornerRadiusX;
	const float m_CornerRadiusY;

	const int m_RoundSegments{ 5 };

	const static float m_Pi;
};