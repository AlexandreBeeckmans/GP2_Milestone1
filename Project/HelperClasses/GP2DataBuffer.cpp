#include "GP2DataBuffer.h"
#include "GP2CommandBuffer.h"
#include "GP2CommandPool.h"

GP2DataBuffer::GP2DataBuffer(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkDevice,
	VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties,
	VkDeviceSize size
) :
	m_VkDevice{ vkDevice },
	m_Size{ size }
{
	CreateBuffer(size, usage, properties, m_VkBuffer, m_VkBufferMemory, vkDevice, vkPhysicalDevice);
}

GP2DataBuffer::GP2DataBuffer(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkDevice,
	VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties,
	VkDeviceSize size,
	const void* bufferData,
	GP2CommandPool commandPool,
	VkQueue graphicsQueue
) :
	m_VkDevice{ vkDevice },
	m_Size{ size }
{
	VkBuffer stagingBuffer{};
	VkDeviceMemory stagingBufferMemory{};
	CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, vkDevice, vkPhysicalDevice);
	

	void* data{};
	vkMapMemory(vkDevice, stagingBufferMemory, 0, size, 0, &data);
	memcpy(data, bufferData, size_t(size));
	vkUnmapMemory(vkDevice, stagingBufferMemory);
	CreateBuffer(size, usage, properties, m_VkBuffer, m_VkBufferMemory, vkDevice, vkPhysicalDevice);

	CopyBuffer(stagingBuffer, m_VkBuffer, size, commandPool, vkDevice, graphicsQueue);

	vkDestroyBuffer(vkDevice, stagingBuffer, nullptr);
	vkFreeMemory(vkDevice, stagingBufferMemory, nullptr);
}


void GP2DataBuffer::Destroy(const VkDevice& vkDevice)
{
	vkDestroyBuffer(vkDevice, m_VkBuffer, nullptr);
	vkFreeMemory(vkDevice, m_VkBufferMemory, nullptr);
}
void GP2DataBuffer::BindAsVertexBuffer(VkCommandBuffer commandBuffer) {
	VkBuffer vertexBuffers[] = { m_VkBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
}
void GP2DataBuffer::BindAsIndexBuffer(VkCommandBuffer commandBuffer)
{
	vkCmdBindIndexBuffer(commandBuffer, m_VkBuffer, 0, VK_INDEX_TYPE_UINT16);
}

uint32_t GP2DataBuffer::FindMemoryType(const uint32_t typeFilter, const VkMemoryPropertyFlags& properties, const VkPhysicalDevice& vkPhysicalDevice)
{
	VkPhysicalDeviceMemoryProperties memoryProperties{};
	vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevice, &memoryProperties);


	for (uint32_t i{ 0 }; i < memoryProperties.memoryTypeCount; ++i)
	{
		if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable mmeory type!");
}

void GP2DataBuffer::CreateBuffer(const VkDeviceSize size, const VkBufferUsageFlags& usage, const VkMemoryPropertyFlags& properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;

	bufferInfo.usage = usage;

	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create vertex buffer!");
	}

	VkMemoryRequirements memoryRequirements{};
	vkGetBufferMemoryRequirements(vkDevice, buffer, &memoryRequirements);

	VkMemoryAllocateInfo allocateInfo{};
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.allocationSize = memoryRequirements.size;
	allocateInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, properties, vkPhysicalDevice);

	if (vkAllocateMemory(vkDevice, &allocateInfo, nullptr, &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}

	vkBindBufferMemory(vkDevice, buffer, bufferMemory, 0);
	m_Size = size;
}
void GP2DataBuffer::CopyBuffer(const VkBuffer& srcBuffer, const VkBuffer& dstBuffer, const VkDeviceSize& size, const GP2CommandPool& commandPool, const VkDevice& vkDevice, const VkQueue& graphicsQueue)
{
	VkCommandBufferAllocateInfo allocateInfo{};
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocateInfo.commandPool = commandPool.GetVkCommandPool();
	allocateInfo.commandBufferCount = 1;

	//VkCommandBuffer commandBuffer;
	GP2CommandBuffer commandBuffer{};
	vkAllocateCommandBuffers(vkDevice, &allocateInfo, const_cast<VkCommandBuffer*>(commandBuffer.GetpVkCommandBuffer()));
	commandBuffer.BeginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer.GetVkCommandBuffer(), srcBuffer, dstBuffer, 1, &copyRegion);

	//vkEndCommandBuffer(commandBuffer);
	commandBuffer.EndRecording();

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	commandBuffer.Submit(submitInfo);

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(vkDevice, commandPool.GetVkCommandPool(), 1, commandBuffer.GetpVkCommandBuffer());
}

void GP2DataBuffer::Upload(UniformBufferObject& ubo)
{
	memcpy(m_UniformBufferMapped, &ubo, m_Size);
}

void GP2DataBuffer::Map(const VkDevice& device)
{
	vkMapMemory(device, m_VkBufferMemory, 0, m_Size, 0, &m_UniformBufferMapped);
}