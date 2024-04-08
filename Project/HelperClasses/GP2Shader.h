#pragma once

#include "vulkan/vulkan_core.h"
#include <vector>
#include <string>
#include <array>
#include <memory>

#include "GP2DataBuffer.h"
#include "Vertex.h"

#include "GP2DescriptorPool.h"


class GP2Shader
{
public:
	GP2Shader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
	~GP2Shader() = default;

	GP2Shader(const GP2Shader&) = delete;
	GP2Shader& operator=(const GP2Shader&) = delete;
	GP2Shader(GP2Shader&&) = delete;
	GP2Shader& operator=(GP2Shader&&) = delete;

	void Initialize(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice);
	void DestroyShaderModules(const VkDevice& vkDevice);
	void DestroyUniformBuffer(const VkDevice& vkDevice);

	const std::vector<VkPipelineShaderStageCreateInfo>& GetShaderStages();

	VkPipelineVertexInputStateCreateInfo GetVertexInputStateInfo();
	VkPipelineInputAssemblyStateCreateInfo createInputAssemblyStateInfo();

	void CreateDescriptorSets(const VkDevice& vkDevice)
	{
		m_DescriptorPool->CreateDescriptorSets(vkDevice, m_DescriptorSetLayout, { m_UBOBuffer->GetVkBuffer() });
	}
	void CreateDescriptorSetLayout(const VkDevice& vkDevice);
	const VkDescriptorSetLayout& GetDescriptorSetLayout()
	{
		return m_DescriptorSetLayout;
	}
	void BindDescriptorSet(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, size_t index);

private:

	VkPipelineShaderStageCreateInfo createFragmentShaderInfo(const VkDevice& vkDevice);
	VkPipelineShaderStageCreateInfo createVertexShaderInfo(const VkDevice& vkDevice);

	VkShaderModule createShaderModule(const VkDevice& vkDevice, const std::vector<char>& code);

	std::string m_VertexShaderFile;
	std::string m_FragmentShaderFile;

	std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;

	//Week 2
	VkPipelineVertexInputStateCreateInfo m_VertexInputInfo{};
	VkVertexInputBindingDescription m_BindingDescription{};
	std::array<VkVertexInputAttributeDescription, 2> m_AttributeDescription{};


	//UBO
	VkDescriptorSetLayout m_DescriptorSetLayout;
	std::unique_ptr<GP2DataBuffer > m_UBOBuffer;
	UniformBufferObject m_UBOSrc;


	std::unique_ptr<GP2DescriptorPool> m_DescriptorPool;
};