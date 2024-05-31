#pragma once
#include <vulkan/vulkan_core.h>
#include "GP2Shader.h"
#include "GP2Scene.h"
#include "GP2Camera.h"
#include "GP2Image.h"

class GP2Pipeline
{
public:
	GP2Pipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, GP2Scene* pScene);
	~GP2Pipeline() = default;

	void Initialize(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice, const GP2CommandPool commandPool, const VkQueue&
	                graphicsQueue, const VkRenderPass& renderPass, GP2CommandBuffer* pCommandBuffer);
	void Cleanup(const VkDevice& vkDevice) const;


	void Record(uint32_t imageIndex, const VkExtent2D& swapChainExtent, const GP2Camera& camera);

	std::string GetVertexShaderPath() const { return m_VertexShaderPath; }
	std::string GetFragmentShaderPath() const { return m_FragmentShaderPath; }

private:

	void DrawFrame(uint32_t imageIndex, const VkExtent2D& swapChainExtent, const GP2Camera& camera);
	void DrawScene(const VkExtent2D& swapChainExtent, const GP2Camera& camera) const;

	void CreateGraphicsPipeline(const VkDevice& vkDevice, const VkRenderPass& renderPass);

	VkPushConstantRange CreatePushConstantRangeVertex() const;
	VkPushConstantRange CreatePushConstantRangeFragment() const;

	VkPipeline m_Pipeline;
	GP2Shader m_Shader;
	GP2CommandBuffer* m_Buffer;
	GP2Scene* m_pScene;

	VkPipelineLayout m_PipelineLayout;

	std::string m_VertexShaderPath;
	std::string m_FragmentShaderPath;
};
