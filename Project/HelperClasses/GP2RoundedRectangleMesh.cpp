#include "GP2RoundedRectangleMesh.h"
#include "vulkanbase/VulkanBase.h"

const float GP2RoundedRectangleMesh::m_Pi = 3.1416f;

GP2RoundedRectangleMesh::GP2RoundedRectangleMesh(const glm::vec2& center, const float width, const float height, const float roundedPercentage) :
	GP2RectangleMesh::GP2RectangleMesh(center, width* (1 - roundedPercentage), height* (1 - roundedPercentage)),
	m_CornerRadiusX{ (width / 2) * roundedPercentage },
	m_CornerRadiusY{ (height / 2) * roundedPercentage }
{
	AddIndex(0);

	const glm::vec2 topLeftCenter
	{
		center.x - (width / 2) * (1 - roundedPercentage),
		center.y - (height / 2) * (1 - roundedPercentage)
	};

	const glm::vec2 topRightCenter
	{
		center.x + (width / 2) * (1 - roundedPercentage),
		center.y - (height / 2) * (1 - roundedPercentage)
	};

	const glm::vec2 bottomLeftCenter
	{
		center.x - (width / 2) * (1 - roundedPercentage),
		center.y + (height / 2) * (1 - roundedPercentage)
	};

	const glm::vec2 bottomRightCenter
	{
		center.x + (width / 2) * (1 - roundedPercentage),
		center.y + (height / 2) * (1 - roundedPercentage)
	};

	//Corners
	CalculateCorner(topLeftCenter, m_Pi);
	CalculateCorner(topRightCenter, 3 * (m_Pi / 2));
	CalculateCorner(bottomLeftCenter, m_Pi / 2);
	CalculateCorner(bottomRightCenter, 0);


	//TopRectangle
	InitRectangleVertices
	(
		glm::vec2{ center.x, center.y - (height - m_CornerRadiusY) / 2.0f },
		width - (2 * m_CornerRadiusX),
		m_CornerRadiusY
	);

	//BottomRectangle
	InitRectangleVertices
	(
		glm::vec2{ center.x, center.y + (height - m_CornerRadiusY) / 2.0f },
		width - (2 * m_CornerRadiusX),
		m_CornerRadiusY
	);

	//LeftRectangle
	InitRectangleVertices
	(
		glm::vec2{ center.x - (width - m_CornerRadiusX) / 2.0f, center.y },
		m_CornerRadiusX,
		height - (2 * m_CornerRadiusY)
	);

	//RightRectangle
	InitRectangleVertices
	(
		glm::vec2{ center.x + (width - m_CornerRadiusX) / 2.0f, center.y },
		m_CornerRadiusX,
		height - (2 * m_CornerRadiusY)
	);


}

void GP2RoundedRectangleMesh::CalculateCorner(const glm::vec2& center, const float startingAngle)
{
	for (int i{ 0 }; i < m_RoundSegments; ++i)
	{
		const float angle{ startingAngle + ((2 * m_Pi) / (m_RoundSegments * 4.0f) * i) };
		const float nextAngle{ startingAngle + ((2 * m_Pi) / (m_RoundSegments * 4.0f) * (i + 1)) };

		AddVertex({ center.x, center.y }, { 1.0f, 0.0f, 0.0f });
		AddVertex({ center.x + std::cosf(angle) * m_CornerRadiusX, center.y + std::sinf(angle) * m_CornerRadiusY }, { 0.0f, std::cosf(angle), std::sinf(angle) });
		AddVertex({ center.x + std::cosf(nextAngle) * m_CornerRadiusX, center.y + std::sinf(nextAngle) * m_CornerRadiusY }, { 0.0f, std::cosf(nextAngle), std::sinf(nextAngle) });
	}
}