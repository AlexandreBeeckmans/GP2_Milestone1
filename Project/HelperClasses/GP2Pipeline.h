#pragma once
#include <vulkan/vulkan_core.h>
#include "GP2Shader.h"
#include "GP2Scene.h"

class GP2Pipeline
{
public:
	GP2Pipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~GP2Pipeline() = default;

	void Initialize(const VkDevice& vkDevice,  const VkPhysicalDevice& vkPhysicalDevice, const VkFormat& swapChainImageFormat, const std::vector<VkImageView>& swapChainImageViews, const VkExtent2D& swapChainExtent, const VkSurfaceKHR& surface, const VkQueue& graphicsQueue);
	void Cleanup(const VkDevice& vkDevice);


	void Record(uint32_t imageIndex, const VkExtent2D& swapChainExtent);
	void Submit(VkSubmitInfo& submitInfo);

private:

	void DrawFrame(uint32_t imageIndex, const VkExtent2D& swapChainExtent);
	void DrawScene();

	void CreateRenderPass(const VkDevice& vkDevice, const VkFormat& swapChainImageFormat);
	void CreateGraphicsPipeline(const VkDevice& vkDevice);
	void CreateFrameBuffers(const VkDevice& vkDevice, const std::vector<VkImageView>& swapChainImageViews, const VkExtent2D& swapChainExtent);

	QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

	VkPipeline m_Pipeline;
	GP2Shader m_Shader;
	GP2CommandBuffer m_Buffer;
	GP2Scene m_Scene;
	GP2CommandPool m_CommandPool;

	VkPipelineLayout m_PipelineLayout;
	VkRenderPass m_RenderPass;

	std::vector<VkFramebuffer> m_SwapChainFramebuffers;
	
};