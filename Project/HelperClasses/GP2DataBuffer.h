#pragma once
#include <stdexcept>
#include "vulkan/vulkan_core.h"

#include "GP2CommandPool.h"
#include "Vertex.h"

class GP2DataBuffer
{
public:

	GP2DataBuffer() = default;
	GP2DataBuffer(
		VkPhysicalDevice physicalDevice,
		VkDevice device,
		VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkDeviceSize size
	);
	GP2DataBuffer(
		VkPhysicalDevice physicalDevice,
		VkDevice device,
		VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkDeviceSize size,
		const void* data,
		GP2CommandPool commandPool,
		VkQueue graphicsQueue
		);

	~GP2DataBuffer() = default;


	void Destroy(const VkDevice& vkDevice);
	void BindAsVertexBuffer(VkCommandBuffer commandBuffer);
	void BindAsIndexBuffer(VkCommandBuffer commandBuffer);

	VkBuffer& GetVkBuffer() 
	{
		return m_VkBuffer;
	};

	VkDeviceMemory& GetVkBufferMemory()
	{
		return m_VkBufferMemory;
	};

	VkDeviceSize GetSizeInBytes() const
	{
		return m_Size;
	};

	void CreateBuffer(const VkDeviceSize size, const VkBufferUsageFlags& usage, const VkMemoryPropertyFlags& properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice);
	void Upload(UniformBufferObject& ubo) const;
	void Map(const VkDevice& device);


	static uint32_t FindMemoryType(const uint32_t typeFilter, const VkMemoryPropertyFlags& properties, const VkPhysicalDevice& vkPhysicalDevice);
private:
	VkDeviceSize m_Size;

	VkBuffer m_VkBuffer;
	VkDeviceMemory m_VkBufferMemory;

	void* m_UniformBufferMapped;


	
	void CopyBuffer(const VkBuffer& srcBuffer, const VkBuffer& dstBuffer, const VkDeviceSize& size, const GP2CommandPool& commandPool, const VkDevice& vkDevice, const VkQueue& graphicsQueue);
};