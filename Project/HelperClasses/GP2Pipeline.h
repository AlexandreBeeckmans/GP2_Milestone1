#pragma once
#include <vulkan/vulkan_core.h>
#include "GP2Shader.h"
#include "GP2Scene.h"

class GP2Pipeline
{
public:
	GP2Pipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, GP2Scene* pScene);
	~GP2Pipeline() = default;

	void Initialize(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice, const GP2CommandPool commandPool, const VkQueue&
	                graphicsQueue, const VkRenderPass& renderPass, GP2CommandBuffer* pCommandBuffer);
	void Cleanup(const VkDevice& vkDevice);


	void Record(uint32_t imageIndex, const VkExtent2D& swapChainExtent);

private:

	void DrawFrame(uint32_t imageIndex, const VkExtent2D& swapChainExtent);
	void DrawScene();

	void CreateGraphicsPipeline(const VkDevice& vkDevice, const VkRenderPass& renderPass);

	VkPipeline m_Pipeline;
	GP2Shader m_Shader;
	GP2CommandBuffer* m_Buffer;
	GP2Scene* m_pScene;

	VkPipelineLayout m_PipelineLayout;
};