#pragma once

#include <vector>
#include "GP2Mesh.h"


class GP2Scene
{
public:
	GP2Scene();
	~GP2Scene() = default;

	void Initialize(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice, const GP2CommandPool& commandPool, const VkQueue& graphicsQueue);
	void Cleanup(const VkDevice& vkDevice);

	void Draw(const VkCommandBuffer& vkCommandBuffer);

	void AddMesh(GP2Mesh& mesh);

private:
	std::vector<GP2Mesh> m_Meshes{};
};