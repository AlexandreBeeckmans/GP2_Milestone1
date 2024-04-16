#include "GP2Mesh.h"
#include <stdexcept>

#include "Vertex.h"
#include "vulkanbase/VulkanBase.h"


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <chrono>

void GP2Mesh::Initialize(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice, const GP2CommandPool& commandPool, const VkQueue& graphicsQueue)
{
	VkDeviceSize vertexBufferSize{ sizeof(m_Vertices[0]) * m_Vertices.size() };
	m_VertexBuffer = std::make_unique<GP2DataBuffer>(vkPhysicalDevice, vkDevice, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBufferSize, m_Vertices.data(), commandPool, graphicsQueue);

	VkDeviceSize indexBufferSize = sizeof(m_Indices[0]) * m_Indices.size();
	m_IndexBuffer = std::make_unique<GP2DataBuffer>(vkPhysicalDevice, vkDevice, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBufferSize, m_Indices.data(), commandPool, graphicsQueue);
}
void GP2Mesh::Destroy(const VkDevice& vkDevice) const
{
	m_VertexBuffer->Destroy(vkDevice);
	m_IndexBuffer->Destroy(vkDevice);
}

void GP2Mesh::Draw(VkCommandBuffer commandBuffer) const
{
	m_VertexBuffer->BindAsVertexBuffer(commandBuffer);
	m_IndexBuffer->BindAsIndexBuffer(commandBuffer);

	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_Indices.size()), 1, 0, 0, 0);
}

void GP2Mesh::AddVertex(glm::vec2 pos, glm::vec3 color, glm::vec3 normal)
{
	AddVertex({ pos.x, pos.y, 0 }, color, normal);
}
void GP2Mesh::AddVertex(glm::vec3 pos, glm::vec3 color, glm::vec3 normal)
{
	Vertex tempVertex{};
	tempVertex.pos = pos;
	tempVertex.color = color;
	tempVertex.normal = normal;
	AddVertex(tempVertex);
}
void GP2Mesh::AddVertex(const Vertex& vertex)
{
	m_Vertices.emplace_back(vertex);
}
void GP2Mesh::AddIndex(size_t value)
{
	m_Indices.push_back(static_cast<uint16_t>(value));
}

int GP2Mesh::GetNumberVertices()const
{
	return int(m_Vertices.size());
}