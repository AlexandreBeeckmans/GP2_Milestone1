#pragma once

#include "vulkan/vulkan_core.h"
#include <vector>
#include <string>
#include <array>


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

	const std::vector<VkPipelineShaderStageCreateInfo>& GetShaderStages();

	VkPipelineVertexInputStateCreateInfo createVertexInputStateInfo();
	VkPipelineInputAssemblyStateCreateInfo createInputAssemblyStateInfo();

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
};