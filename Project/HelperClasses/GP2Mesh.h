#pragma once
#include <vulkan/vulkan_core.h>

#include <vector>
#include "glm/glm.hpp"

#include "GP2CommandPool.h"


struct Vertex;

class GP2Mesh
{
public:

	GP2Mesh();

	void Initialize(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice, const GP2CommandPool& commandPool, const VkQueue& graphicsQueue);
	void Destroy(const VkDevice& vkDevice);

	void Draw(VkCommandBuffer commandBuffer) const;
	

	void AddVertex(glm::vec2 pos, glm::vec3 color);
	void AddIndex(const uint16_t value);


private:

	void CreateVertexBuffer(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice, const GP2CommandPool& commandPool, const VkQueue& graphicsQueue);
	void CreateBuffer(const VkDeviceSize size, const VkBufferUsageFlags& usage, const VkMemoryPropertyFlags& properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice);
	void CopyBuffer(const VkBuffer& srcBuffer, const VkBuffer& dstBuffer, const VkDeviceSize& size, const GP2CommandPool& commandPool, const VkDevice& vkDevice, const VkQueue& graphicsQueue);

	void CreateIndexBuffer(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice, const GP2CommandPool& commandPool, const VkQueue& graphicsQueue);

	uint32_t FindMemoryType(const uint32_t typeFilter, const VkMemoryPropertyFlags& properties, const VkPhysicalDevice& vkPhysicalDevice);

	VkBuffer m_VertexBuffer{};
	VkDeviceMemory m_VertexBufferMemory{};
	std::vector<Vertex> m_Vertices{};


	VkBuffer m_IndexBuffer{};
	VkDeviceMemory m_IndexBufferMemory{};
	std::vector<uint16_t> m_Indices{};

};