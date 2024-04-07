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
	VkDeviceSize vertexBufferSize{ sizeof(m_Vertices[0]) * m_Vertices.size() };
	m_VertexBuffer = std::make_unique<GP2DataBuffer>(vkPhysicalDevice, vkDevice, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBufferSize, m_Vertices.data(), commandPool, graphicsQueue);

	VkDeviceSize indexBufferSize = sizeof(m_Indices[0]) * m_Indices.size();
	m_IndexBuffer = std::make_unique<GP2DataBuffer>(vkPhysicalDevice, vkDevice, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBufferSize, m_Indices.data(), commandPool, graphicsQueue);

	CreateUniformBuffers(vkDevice, vkPhysicalDevice);
}
void GP2Mesh::Destroy(const VkDevice& vkDevice)
{
	m_VertexBuffer->Destroy(vkDevice);
	m_IndexBuffer->Destroy(vkDevice);
	for (size_t i{ 0 }; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		m_UniformBuffers[i].Destroy(vkDevice);
	}
}

void GP2Mesh::Draw(VkCommandBuffer commandBuffer) const
{
	m_VertexBuffer->BindAsVertexBuffer(commandBuffer);
	m_IndexBuffer->BindAsIndexBuffer(commandBuffer);

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

void GP2Mesh::CreateUniformBuffers(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice)
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	m_UniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	m_UniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i{ 0 }; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		m_UniformBuffers[i].CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_UniformBuffers[i].GetVkBuffer(), m_UniformBuffers[i].GetVkBufferMemory(), vkDevice, vkPhysicalDevice);
		vkMapMemory(vkDevice, m_UniformBuffers[i].GetVkBufferMemory(), 0, bufferSize, 0, &m_UniformBuffersMapped[i]);
	}
}