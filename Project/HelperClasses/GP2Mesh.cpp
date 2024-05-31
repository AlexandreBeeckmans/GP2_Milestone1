#include "GP2Mesh.h"
#include <stdexcept>

#include "Vertex.h"
#include "vulkanbase/VulkanBase.h"


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <chrono>

GP2Mesh::GP2Mesh(const glm::vec3& position) :
m_Position(position),
m_VertexConstant()
{
}

void GP2Mesh::Initialize(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice, const GP2CommandPool& commandPool, const VkQueue& graphicsQueue)
{
	VkDeviceSize vertexBufferSize{ sizeof(m_Vertices[0]) * m_Vertices.size() };
	m_VertexBuffer = std::make_unique<GP2DataBuffer>(vkPhysicalDevice, vkDevice, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBufferSize, m_Vertices.data(), commandPool, graphicsQueue);

	VkDeviceSize indexBufferSize = sizeof(m_Indices[0]) * m_Indices.size();
	m_IndexBuffer = std::make_unique<GP2DataBuffer>(vkPhysicalDevice, vkDevice, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBufferSize, m_Indices.data(), commandPool, graphicsQueue);

	m_VertexConstant = glm::identity<glm::mat4>();
}
void GP2Mesh::Destroy(const VkDevice& vkDevice) const
{
	m_VertexBuffer->Destroy(vkDevice);
	m_IndexBuffer->Destroy(vkDevice);

	m_Shader->DestroyShaderModules(vkDevice);
	m_Shader->DestroyUniformBuffer(vkDevice);
	vkDestroyDescriptorSetLayout(vkDevice, m_Shader->GetDescriptorSetLayout(), nullptr);

	m_TextureMap->Destroy(vkDevice);
	m_NormalMap->Destroy(vkDevice);
	m_SpecularMap->Destroy(vkDevice);
	m_GlossMap->Destroy(vkDevice);
}

void GP2Mesh::Draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, const VkExtent2D& swapChainExtent, const GP2Camera& camera) const
{
	m_VertexBuffer->BindAsVertexBuffer(commandBuffer);
	m_IndexBuffer->BindAsIndexBuffer(commandBuffer);

	m_Shader->BindDescriptorSet(commandBuffer, pipelineLayout, 0);

	vkCmdPushConstants(
		commandBuffer,
		pipelineLayout,
		VK_SHADER_STAGE_VERTEX_BIT, // Stage flag should match the push constan range in the layout
		0, // Offset within the push constant block
		sizeof(MeshData), // Size of the push constants to update
		&m_Constants); // Pointer to the data);

	
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_Indices.size()), 1, 0, 0, 0);

	m_Shader->UpdateUniformBuffer(0, swapChainExtent.width / (float)swapChainExtent.height, 45.f, camera);
}

void GP2Mesh::AddVertex(glm::vec2 pos, glm::vec3 color, glm::vec3 normal, glm::vec2 uv, glm::vec3 tangent)
{
	AddVertex({ m_Position.x + pos.x, m_Position.y + pos.y, 0 }, color, normal, uv, tangent);
}
void GP2Mesh::AddVertex(glm::vec3 pos, glm::vec3 color, glm::vec3 normal, glm::vec2 uv, glm::vec3 tangent)
{
	Vertex3D tempVertex{};
	tempVertex.pos = pos;
	tempVertex.color = color;
	tempVertex.normal = normal;
	tempVertex.uv = uv;
	tempVertex.tangent = tangent;
	AddVertex(tempVertex);
}
void GP2Mesh::AddVertex(const Vertex3D& vertex)
{
	m_Vertices.emplace_back(vertex);
}
void GP2Mesh::AddIndex(size_t value)
{
	m_Indices.push_back(static_cast<uint16_t>(value));
}

void GP2Mesh::Update(const glm::vec3& cameraPosition)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	m_VertexConstant = glm::identity<glm::mat4>();
	m_VertexConstant = glm::translate(m_VertexConstant, m_Position);
	m_VertexConstant = glm::rotate(m_VertexConstant, time * glm::radians(45.0f), glm::vec3(0.0f, 1.0f, .0f));
	m_VertexConstant = glm::translate(m_VertexConstant, -m_Position);

	m_CameraPositionConstant = cameraPosition;

	m_Constants = { m_VertexConstant, m_CameraPositionConstant };
}

void GP2Mesh::InitShader(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice, const std::string& vertexShaderPath, const std::string fragmentShaderPath)
{
	m_Shader = std::make_unique<GP2Shader>(vertexShaderPath, fragmentShaderPath);
	m_Shader->Initialize(vkDevice, vkPhysicalDevice);

	m_Shader->CreateDescriptorSetLayout(vkDevice);
	m_Shader->CreateDescriptorSets(vkDevice, *m_TextureMap, *m_NormalMap, *m_SpecularMap, *m_GlossMap);
}

void GP2Mesh::SetTextureMap(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice, const GP2CommandPool& commandPool, const VkQueue& graphicsQueue, const std::string& textureMapPath)
{
	m_TextureMap = std::make_unique<GP2Image>();
	m_TextureMap->Initialize(vkDevice, vkPhysicalDevice, commandPool, graphicsQueue, textureMapPath);
}

void GP2Mesh::SetNormalMap(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice,
	const GP2CommandPool& commandPool, const VkQueue& graphicsQueue, const std::string& textureMapPath)
{
	m_NormalMap = std::make_unique<GP2Image>();
	m_NormalMap->Initialize(vkDevice, vkPhysicalDevice, commandPool, graphicsQueue, textureMapPath);
}

void GP2Mesh::SetSpecularMap(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice,
	const GP2CommandPool& commandPool, const VkQueue& graphicsQueue, const std::string& textureMapPath)
{
	m_SpecularMap = std::make_unique<GP2Image>();
	m_SpecularMap->Initialize(vkDevice, vkPhysicalDevice, commandPool, graphicsQueue, textureMapPath);
}

void GP2Mesh::SetGlossMap(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice,
	const GP2CommandPool& commandPool, const VkQueue& graphicsQueue, const std::string& textureMapPath)
{
	m_GlossMap = std::make_unique<GP2Image>();
	m_GlossMap->Initialize(vkDevice, vkPhysicalDevice, commandPool, graphicsQueue, textureMapPath);
}

int GP2Mesh::GetNumberVertices()const
{
	return static_cast<int>(m_Vertices.size());
}
