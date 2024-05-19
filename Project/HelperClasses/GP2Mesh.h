#pragma once
#include <vulkan/vulkan_core.h>

#include <vector>
#include <memory>
#include "glm/glm.hpp"

#include "GP2CommandPool.h"
#include "GP2DataBuffer.h"


struct Vertex3D;

class GP2Mesh
{
public:

	GP2Mesh(const glm::vec3& position = {0,0,0});

	void Initialize(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice, const GP2CommandPool& commandPool, const VkQueue& graphicsQueue);
	void Destroy(const VkDevice& vkDevice) const;

	void Draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) const;
	
	void AddVertex(glm::vec2 pos, glm::vec3 color, glm::vec3 normal = {0,0,0 }, glm::vec2 uv = {0,0}, glm::vec3 tangent = { 0,0,0 });
	void AddVertex(glm::vec3 pos, glm::vec3 color, glm::vec3 normal = {0,0,0 }, glm::vec2 uv = {0,0}, glm::vec3 tangent = {0,0,0});
	void AddVertex(const Vertex3D& vertex);
	void AddIndex(size_t value);
	void Update(const glm::vec3& cameraPosition);

protected:
	int GetNumberVertices()const;
	glm::vec3 m_Position;

private:
	std::unique_ptr<GP2DataBuffer> m_VertexBuffer{};
	std::vector<Vertex3D> m_Vertices{};

	std::unique_ptr<GP2DataBuffer> m_IndexBuffer{};
	std::vector<uint16_t> m_Indices{};

	glm::mat4 m_VertexConstant;
	glm::vec3 m_CameraPositionConstant;
	MeshData m_Constants{};
	
};