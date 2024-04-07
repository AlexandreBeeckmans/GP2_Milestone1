#pragma once
#include <vulkan/vulkan_core.h>

#include <vector>
#include <memory>
#include "glm/glm.hpp"

#include "GP2CommandPool.h"
#include "GP2DataBuffer.h"


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

	void UpdateUniformBuffer(const uint32_t& currentImage, const VkExtent2D& swapChainExtent);

protected:
	int GetNumberVertices()const;



private:
	std::unique_ptr<GP2DataBuffer> m_VertexBuffer{};
	std::vector<Vertex> m_Vertices{};

	std::unique_ptr<GP2DataBuffer> m_IndexBuffer{};
	std::vector<uint16_t> m_Indices{};



	std::vector<VkBuffer> m_UniformBuffers;
	std::vector<VkDeviceMemory> m_UniformBuffersMemory;
	std::vector<void*> m_UniformBuffersMapped;
};