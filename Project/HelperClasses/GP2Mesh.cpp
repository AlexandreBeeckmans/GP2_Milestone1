#include "GP2Mesh.h"
#include <stdexcept>

#include "Vertex.h"
#include "vulkanbase/VulkanBase.h"


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <chrono>

GP2Mesh::GP2Mesh()
{
}

void GP2Mesh::Initialize(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice, const GP2CommandPool& commandPool, const VkQueue& graphicsQueue)
{
	CreateVertexBuffer(vkDevice, vkPhysicalDevice, commandPool, graphicsQueue);
	CreateIndexBuffer(vkDevice, vkPhysicalDevice, commandPool, graphicsQueue);
	CreateUniformBuffers(vkDevice, vkPhysicalDevice);
}
void GP2Mesh::Destroy(const VkDevice& vkDevice)
{
	vkDestroyBuffer(vkDevice, m_VertexBuffer, nullptr);
	vkFreeMemory(vkDevice, m_VertexBufferMemory, nullptr);

	vkDestroyBuffer(vkDevice, m_IndexBuffer, nullptr);
	vkFreeMemory(vkDevice, m_IndexBufferMemory, nullptr);

	for (size_t i{ 0 }; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		vkDestroyBuffer(vkDevice, m_UniformBuffers[i], nullptr);
		vkFreeMemory(vkDevice, m_UniformBuffersMemory[i], nullptr);
	}
}

void GP2Mesh::Draw(VkCommandBuffer commandBuffer) const
{
	VkBuffer vertexBuffers[] = { m_VertexBuffer };
	VkDeviceSize offsets[] = { 0 };


	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_Indices.size()), 1, 0, 0, 0);
}

void GP2Mesh::AddVertex(glm::vec2 pos, glm::vec3 color)
{
	m_Vertices.push_back({ pos, color });
}
void GP2Mesh::AddIndex(const uint16_t value)
{
	m_Indices.push_back(value);
}
void GP2Mesh::UpdateUniformBuffer(const uint32_t& currentImage, const VkExtent2D& swapChainExtent)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / float(swapChainExtent.height), 0.1f, 10.0f);

	ubo.proj[1][1] *= -1;

	memcpy(m_UniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

int GP2Mesh::GetNumberVertices()const
{
	return int(m_Vertices.size());
}

void GP2Mesh::CreateVertexBuffer(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice, const GP2CommandPool& commandPool, const VkQueue& graphicsQueue)
{

	VkDeviceSize bufferSize{ sizeof(m_Vertices[0]) * m_Vertices.size() };

	VkBuffer stagingBuffer{};
	VkDeviceMemory stagingBufferMemory{};
	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, vkDevice, vkPhysicalDevice);


	void* data{};
	vkMapMemory(vkDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, m_Vertices.data(), size_t(bufferSize));
	vkUnmapMemory(vkDevice, stagingBufferMemory);
	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory, vkDevice, vkPhysicalDevice);

	CopyBuffer(stagingBuffer, m_VertexBuffer, bufferSize, commandPool, vkDevice, graphicsQueue);

	vkDestroyBuffer(vkDevice, stagingBuffer, nullptr);
	vkFreeMemory(vkDevice, stagingBufferMemory, nullptr);

}
void GP2Mesh::CreateBuffer(const VkDeviceSize size, const VkBufferUsageFlags& usage, const VkMemoryPropertyFlags& properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice)
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
}
void GP2Mesh::CopyBuffer(const VkBuffer& srcBuffer, const VkBuffer& dstBuffer, const VkDeviceSize& size, const GP2CommandPool& commandPool, const VkDevice& vkDevice, const VkQueue& graphicsQueue)
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

void GP2Mesh::CreateIndexBuffer(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice, const GP2CommandPool& commandPool, const VkQueue& graphicsQueue)
{
	VkDeviceSize bufferSize = sizeof(m_Indices[0]) * m_Indices.size();
	VkBuffer stagingBuffer{};
	VkDeviceMemory stagingBufferMemory{};
	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, vkDevice, vkPhysicalDevice);

	void* data{};
	vkMapMemory(vkDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, m_Indices.data(), size_t(bufferSize));
	vkUnmapMemory(vkDevice, stagingBufferMemory);
	CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_IndexBuffer, m_IndexBufferMemory, vkDevice, vkPhysicalDevice);

	CopyBuffer(stagingBuffer, m_IndexBuffer, bufferSize, commandPool, vkDevice, graphicsQueue);
	vkDestroyBuffer(vkDevice, stagingBuffer, nullptr);
	vkFreeMemory(vkDevice, stagingBufferMemory, nullptr);
}

void GP2Mesh::CreateUniformBuffers(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice)
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	m_UniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	m_UniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
	m_UniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i{ 0 }; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_UniformBuffers[i], m_UniformBuffersMemory[i], vkDevice, vkPhysicalDevice);
		vkMapMemory(vkDevice, m_UniformBuffersMemory[i], 0, bufferSize, 0, &m_UniformBuffersMapped[i]);
	}
}

uint32_t GP2Mesh::FindMemoryType(const uint32_t typeFilter, const VkMemoryPropertyFlags& properties, const VkPhysicalDevice& vkPhysicalDevice)
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