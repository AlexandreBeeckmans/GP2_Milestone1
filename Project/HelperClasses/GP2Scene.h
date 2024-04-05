#pragma once

#include <vector>
#include "GP2Mesh.h"


class GP2Scene final
{
public:
	GP2Scene();
	~GP2Scene() = default;

	void Initialize(const VkPhysicalDevice& vkPhysicalDevice, const VkDevice& vKDevice);
	void Cleanup(const VkDevice& vkDevice);
	void Draw(const VkCommandBuffer& vkCommandBuffer);

private:
	std::vector<GP2Mesh> m_Meshes{};
};