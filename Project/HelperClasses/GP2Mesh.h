#pragma once
#include <vulkan/vulkan_core.h>

#include <vector>
#include "glm/glm.hpp"


struct Vertex;

class GP2Mesh
{
public:

	GP2Mesh();

	void Initialize(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice);
	void Destroy(const VkDevice& vkDevice);

	void Draw(VkCommandBuffer commandBuffer) const;
	

	void AddVertex(glm::vec2 pos, glm::vec3 color);


private:

	uint32_t FindMemoryType(const uint32_t typeFilter, const VkMemoryPropertyFlags& properties, const VkPhysicalDevice& vkPhysicalDevice);

	VkBuffer m_VertexBuffer{};
	VkDeviceMemory m_VertexBufferMemory{};
	std::vector<Vertex> m_Vertices{};
};