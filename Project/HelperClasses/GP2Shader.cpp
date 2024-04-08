#include "GP2Shader.h"
#include <vulkanbase/VulkanUtil.h>

#include "HelperClasses/Vertex.h"

GP2Shader::GP2Shader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile):
	m_VertexShaderFile{ vertexShaderFile },
	m_FragmentShaderFile{ fragmentShaderFile }
{
	m_BindingDescription = Vertex::getBindingDescription();
	m_AttributeDescription = Vertex::getAttributeDescriptions();
	m_VertexInputInfo = Vertex::GetVertexInputInfo(m_BindingDescription, m_AttributeDescription);
}

void GP2Shader::Initialize(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice)
{
	m_ShaderStages.push_back(createFragmentShaderInfo(vkDevice));
	m_ShaderStages.push_back(createVertexShaderInfo(vkDevice));


	m_UBOBuffer = std::make_unique<GP2DataBuffer>(vkPhysicalDevice,
		vkDevice,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		sizeof(UniformBufferObject));

	m_DescriptorPool = std::make_unique<GP2DescriptorPool>(vkDevice, 0, 1);
}
void GP2Shader::DestroyShaderModules(const VkDevice& vkDevice)
{
	for (VkPipelineShaderStageCreateInfo& stageInfo : m_ShaderStages)
	{
		vkDestroyShaderModule(vkDevice, stageInfo.module, nullptr);
	}
	m_ShaderStages.clear();
}

void GP2Shader::DestroyUniformBuffer(const VkDevice& vkDevice)
{
	m_UBOBuffer->Destroy(vkDevice);
	m_DescriptorPool->DestroyPool(vkDevice);
}

const std::vector<VkPipelineShaderStageCreateInfo>& GP2Shader::GetShaderStages()
{
	return m_ShaderStages;
}

VkPipelineVertexInputStateCreateInfo GP2Shader::GetVertexInputStateInfo()
{
	return m_VertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo GP2Shader::createInputAssemblyStateInfo()
{
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	return inputAssembly;
}

VkPipelineShaderStageCreateInfo GP2Shader::createFragmentShaderInfo(const VkDevice& vkDevice)
{
	std::vector<char> fragShaderCode = readFile(m_FragmentShaderFile);
	VkShaderModule fragShaderModule = createShaderModule(vkDevice, fragShaderCode);

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	return fragShaderStageInfo;
}
VkPipelineShaderStageCreateInfo GP2Shader::createVertexShaderInfo(const VkDevice& vkDevice)
{
	std::vector<char> vertShaderCode = readFile(m_VertexShaderFile);
	VkShaderModule vertShaderModule = createShaderModule(vkDevice, vertShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";
	return vertShaderStageInfo;
}

VkShaderModule GP2Shader::createShaderModule(const VkDevice& vkDevice, const std::vector<char>& code)
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
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;

	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; //Optional

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;

	if (vkCreateDescriptorSetLayout(vkDevice, &layoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor set layout");
	}
}

void GP2Shader::BindDescriptorSet(VkCommandBuffer buffer, VkPipelineLayout layout, size_t index)
{
	m_DescriptorPool->BindDescriptorSet(buffer, layout, index);
}