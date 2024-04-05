#include "GP2Mesh.h"
#include <stdexcept>

#include "Vertex.h"

GP2Mesh::GP2Mesh()
{
	AddVertex({ -0.25f, -0.25f }, { 1.0f, 0.0f, 0.0f });
	AddVertex({ 0.25f, -0.25f }, { 0.0f, 1.0f, 0.0f });
	AddVertex({ -0.25f, 0.25f }, { 0.0f, 0.0f, 1.0f });
	AddVertex({ -0.25f, 0.25f }, { 0.0f, 0.0f, 1.0f });
	AddVertex({ 0.25f, -0.25f }, { 0.0f, 1.0f, 0.0f });
	AddVertex({ 0.25f, 0.25f }, { 1.0f, 0.0f, 0.0f });
}

void GP2Mesh::Initialize(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(m_Vertices[0]) * m_Vertices.size();

	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &m_VertexBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create vertex buffer!");
	}

	VkMemoryRequirements memoryRequirements{};
	vkGetBufferMemoryRequirements(vkDevice, m_VertexBuffer, &memoryRequirements);

	VkMemoryAllocateInfo allocateInfo{};
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.allocationSize = memoryRequirements.size;
	allocateInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vkPhysicalDevice);

	if (vkAllocateMemory(vkDevice, &allocateInfo, nullptr, &m_VertexBufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate vertex buffer memory!");
	}

	vkBindBufferMemory(vkDevice, m_VertexBuffer, m_VertexBufferMemory, 0);

	void* data{};
	vkMapMemory(vkDevice, m_VertexBufferMemory, 0, bufferInfo.size, 0, &data);
	memcpy(data, m_Vertices.data(), size_t(bufferInfo.size));
	vkUnmapMemory(vkDevice, m_VertexBufferMemory);
}
void GP2Mesh::Destroy(const VkDevice& vkDevice)
{
	vkDestroyBuffer(vkDevice, m_VertexBuffer, nullptr);
	vkFreeMemory(vkDevice, m_VertexBufferMemory, nullptr);
}

void GP2Mesh::Draw(VkCommandBuffer commandBuffer) const
{
	VkBuffer vertexBuffers[] = { m_VertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdDraw(commandBuffer, static_cast<uint32_t>(m_Vertices.size()), 1, 0, 0);
}

void GP2Mesh::AddVertex(glm::vec2 pos, glm::vec3 color)
{
	m_Vertices.push_back({ pos, color });
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