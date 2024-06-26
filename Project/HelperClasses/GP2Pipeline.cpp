#include "GP2Pipeline.h"
#include<vulkanbase/VulkanBase.h>

GP2Pipeline::GP2Pipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, GP2Scene* pScene):
	m_Pipeline{},
	m_Shader{ vertexShaderPath, fragmentShaderPath },
	m_Buffer{},
	m_pScene{pScene},
	m_PipelineLayout{},
	m_VertexShaderPath(vertexShaderPath),
	m_FragmentShaderPath(fragmentShaderPath)
{

}


void GP2Pipeline::Initialize(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice, const GP2CommandPool commandPool, const VkQueue& graphicsQueue, const VkRenderPass& renderPass, GP2CommandBuffer* pCommandBuffer)
{
	m_Shader.Initialize(vkDevice, vkPhysicalDevice);
	m_Shader.CreateDescriptorSetLayout(vkDevice);

	//m_Shader.CreateDescriptorSets(vkDevice, texture, normalMap, specularMap, glossMap);

	CreateGraphicsPipeline(vkDevice, renderPass);

	m_pScene->Initialize(vkDevice, vkPhysicalDevice, commandPool, graphicsQueue);
	m_Buffer = pCommandBuffer;
}
void GP2Pipeline::Cleanup(const VkDevice& vkDevice) const
{
	m_pScene->Cleanup(vkDevice);

	m_Shader.DestroyUniformBuffer(vkDevice);
	vkDestroyDescriptorSetLayout(vkDevice, m_Shader.GetDescriptorSetLayout(), nullptr);

	vkDestroyPipeline(vkDevice, m_Pipeline, nullptr);
	vkDestroyPipelineLayout(vkDevice, m_PipelineLayout, nullptr);
}

void GP2Pipeline::Record(uint32_t imageIndex, const VkExtent2D& swapChainExtent, const GP2Camera& camera)
{

	//m_Shader.BindDescriptorSet(m_Buffer->GetVkCommandBuffer(), m_PipelineLayout, 0);
	DrawFrame(imageIndex, swapChainExtent, camera);
	//m_Shader.UpdateUniformBuffer(imageIndex, swapChainExtent.width / (float)swapChainExtent.height, 45.f, camera);
}

void GP2Pipeline::CreateGraphicsPipeline(const VkDevice& vkDevice, const VkRenderPass& renderPass)
{
	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_NONE;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};
	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;

	pipelineLayoutInfo.pSetLayouts = &m_Shader.GetDescriptorSetLayout();
	


	VkPushConstantRange pushConstantRangeVertex = CreatePushConstantRangeVertex();
	VkPushConstantRange pushConstantRangeFragment = CreatePushConstantRangeFragment();
	std::array<VkPushConstantRange, 2> pushConstantRanges = { pushConstantRangeVertex, pushConstantRangeFragment };
	pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size());

	pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges.data();

	if (vkCreatePipelineLayout(vkDevice, &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}


	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f;
	depthStencil.maxDepthBounds = 1.0f;
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {};
	depthStencil.back = {};


	VkGraphicsPipelineCreateInfo pipelineInfo{};

	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

	pipelineInfo.stageCount = (uint32_t)m_Shader.GetShaderStages().size();
	pipelineInfo.pStages = m_Shader.GetShaderStages().data();
	pipelineInfo.pVertexInputState = &m_Shader.GetVertexInputStateInfo();
	pipelineInfo.pInputAssemblyState = &m_Shader.CreateInputAssemblyStateInfo();

	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.layout = m_PipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(vkDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}

	m_Shader.DestroyShaderModules(vkDevice);
}

VkPushConstantRange GP2Pipeline::CreatePushConstantRangeVertex() const
{
		VkPushConstantRange pushConstantRange = {};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // Stage the pus constant is accessible from
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(MeshData); // Size of push constant block


		return pushConstantRange;
}

VkPushConstantRange GP2Pipeline::CreatePushConstantRangeFragment() const
{
	VkPushConstantRange pushConstantRange = {};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; // Stage the pus constant is accessible from
	pushConstantRange.offset = sizeof(MeshData);
	pushConstantRange.size = sizeof(ToggleMapData); // Size of push constant block


	return pushConstantRange;
}

void GP2Pipeline::DrawFrame(uint32_t imageIndex, const VkExtent2D& swapChainExtent, const GP2Camera& camera)
{

	vkCmdBindPipeline(m_Buffer->GetVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline);

	VkViewport viewport;
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(m_Buffer->GetVkCommandBuffer(), 0, 1, &viewport);

	VkRect2D scissor;
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(m_Buffer->GetVkCommandBuffer(), 0, 1, &scissor);

	DrawScene(swapChainExtent, camera);
}
void GP2Pipeline::DrawScene(const VkExtent2D& swapChainExtent, const GP2Camera& camera) const
{
	m_pScene->Draw(m_Buffer->GetVkCommandBuffer(), m_PipelineLayout, swapChainExtent, camera);
}
