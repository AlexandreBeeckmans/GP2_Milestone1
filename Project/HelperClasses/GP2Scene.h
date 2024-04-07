#pragma once

#include <vector>
#include "GP2Mesh.h"


class GP2Scene final
{
public:
	GP2Scene();
	~GP2Scene() = default;

	void Initialize(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice, const GP2CommandPool& commandPool, const VkQueue& graphicsQueue);
	void Cleanup(const VkDevice& vkDevice);

	void Draw(const VkCommandBuffer& vkCommandBuffer);
	void UpdateUniformBuffers(const uint32_t& currentImage, const VkExtent2D& swapChainExtent);

private:
	std::vector<GP2Mesh> m_Meshes{};
};