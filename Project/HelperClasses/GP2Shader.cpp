#include "GP2Shader.h"
#include <vulkanbase/VulkanUtil.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>


#include "GP2Camera.h"
#include "HelperClasses/Vertex.h"

GP2Shader::GP2Shader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile):
	m_VertexShaderFile{ vertexShaderFile },
	m_FragmentShaderFile{ fragmentShaderFile },
	m_DescriptorSetLayout{},
	m_UBOSrc{}
{
	m_BindingDescription = Vertex3D::getBindingDescription();
	m_AttributeDescription = Vertex3D::getAttributeDescriptions();
	m_VertexInputInfo = Vertex3D::GetVertexInputInfo(m_BindingDescription, m_AttributeDescription);
}

void GP2Shader::Initialize(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice)
{
	m_ShaderStages.push_back(CreateFragmentShaderInfo(vkDevice));
	m_ShaderStages.push_back(CreateVertexShaderInfo(vkDevice));


	m_UBOBuffer = std::make_unique<GP2DataBuffer>(vkPhysicalDevice,
		vkDevice,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		sizeof(UniformBufferObject));
	m_UBOBuffer->Map(vkDevice);

	m_DescriptorPool = std::make_unique<GP2DescriptorPool>(vkDevice, 1);
}
void GP2Shader::DestroyShaderModules(const VkDevice& vkDevice)
{
	for (VkPipelineShaderStageCreateInfo& stageInfo : m_ShaderStages)
	{
		vkDestroyShaderModule(vkDevice, stageInfo.module, nullptr);
	}
	m_ShaderStages.clear();
}

void GP2Shader::DestroyUniformBuffer(const VkDevice& vkDevice) const
{
	m_UBOBuffer->Destroy(vkDevice);
	m_DescriptorPool->DestroyPool(vkDevice);
}

const std::vector<VkPipelineShaderStageCreateInfo>& GP2Shader::GetShaderStages()
{
	return m_ShaderStages;
}

VkPipelineVertexInputStateCreateInfo GP2Shader::GetVertexInputStateInfo() const
{
	return m_VertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo GP2Shader::CreateInputAssemblyStateInfo()
{
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	return inputAssembly;
}

VkPipelineShaderStageCreateInfo GP2Shader::CreateFragmentShaderInfo(const VkDevice& vkDevice) const
{
	std::vector<char> fragShaderCode = readFile(m_FragmentShaderFile);
	VkShaderModule fragShaderModule = CreateShaderModule(vkDevice, fragShaderCode);

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	return fragShaderStageInfo;
}
VkPipelineShaderStageCreateInfo GP2Shader::CreateVertexShaderInfo(const VkDevice& vkDevice) const
{
	std::vector<char> vertShaderCode = readFile(m_VertexShaderFile);
	VkShaderModule vertShaderModule = CreateShaderModule(vkDevice, vertShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";
	return vertShaderStageInfo;
}

VkShaderModule GP2Shader::CreateShaderModule(const VkDevice& vkDevice, const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(vkDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}

void GP2Shader::CreateDescriptorSetLayout(const VkDevice& vkDevice)
{
	VkDescriptorSetLayoutBinding uboLayoutBinding;
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;

	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; //Optional

	VkDescriptorSetLayoutBinding samplerLayoutBinding;
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding samplerNormalLayoutBinding;
	samplerNormalLayoutBinding.binding = 2;
	samplerNormalLayoutBinding.descriptorCount = 1;
	samplerNormalLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerNormalLayoutBinding.pImmutableSamplers = nullptr;
	samplerNormalLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 3> bindings = { uboLayoutBinding, samplerLayoutBinding, samplerNormalLayoutBinding };
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(vkDevice, &layoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor set layout");
	}
}

void GP2Shader::BindDescriptorSet(VkCommandBuffer buffer, VkPipelineLayout layout, size_t index)const
{
	m_DescriptorPool->BindDescriptorSet(buffer, layout, index);
}

void GP2Shader::UpdateUniformBuffer(uint32_t currentImage, float aspectRatio, float fov, const GP2Camera& camera)
{
	m_UBOSrc.view = camera.GetViewMatrix();
	m_UBOSrc.proj = camera.GetProjectionMatrix();

	m_UBOSrc.proj[1][1] *= -1;

	m_UBOBuffer->Upload(m_UBOSrc);
}