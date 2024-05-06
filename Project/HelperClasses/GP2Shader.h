#pragma once

#include "vulkan/vulkan_core.h"
#include <vector>
#include <string>
#include <array>
#include <memory>

#include "GP2Camera.h"
#include "GP2DataBuffer.h"
#include "Vertex.h"

#include "GP2DescriptorPool.h"


class GP2Shader final
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
	void DestroyUniformBuffer(const VkDevice& vkDevice) const;

	const std::vector<VkPipelineShaderStageCreateInfo>& GetShaderStages();

	VkPipelineVertexInputStateCreateInfo GetVertexInputStateInfo() const;
	static VkPipelineInputAssemblyStateCreateInfo CreateInputAssemblyStateInfo();

	void CreateDescriptorSets(const VkDevice& vkDevice, const GP2Image& textureMap, const GP2Image& normalMap, const GP2Image& specularMap = {}, const GP2Image& glossMap = {}) const
	{
		m_DescriptorPool->CreateDescriptorSets(vkDevice, m_DescriptorSetLayout, { m_UBOBuffer->GetVkBuffer() }, textureMap, normalMap, specularMap, glossMap);
	}
	void CreateDescriptorSetLayout(const VkDevice& vkDevice);
	const VkDescriptorSetLayout& GetDescriptorSetLayout() const
	{
		return m_DescriptorSetLayout;
	}
	void BindDescriptorSet(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, size_t index) const;
	void UpdateUniformBuffer(uint32_t currentImage, float aspectRatio, float fov, const GP2Camera& camera);

private:

	VkPipelineShaderStageCreateInfo CreateFragmentShaderInfo(const VkDevice& vkDevice) const;
	VkPipelineShaderStageCreateInfo CreateVertexShaderInfo(const VkDevice& vkDevice) const;

	static VkShaderModule CreateShaderModule(const VkDevice& vkDevice, const std::vector<char>& code);

	std::string m_VertexShaderFile;
	std::string m_FragmentShaderFile;

	std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;

	//Week 2
	VkPipelineVertexInputStateCreateInfo m_VertexInputInfo{};
	VkVertexInputBindingDescription m_BindingDescription{};
	std::array<VkVertexInputAttributeDescription, 5> m_AttributeDescription{};


	//UBO
	VkDescriptorSetLayout m_DescriptorSetLayout;
	std::unique_ptr<GP2DataBuffer > m_UBOBuffer;
	UniformBufferObject m_UBOSrc;


	std::unique_ptr<GP2DescriptorPool> m_DescriptorPool;
};