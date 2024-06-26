#pragma once
#include <vector>

#include "GP2Image.h"
#include "vulkanbase/VulkanUtil.h"

class GP2DescriptorPool final
{
public:
	GP2DescriptorPool(const VkDevice& device, size_t count);
	~GP2DescriptorPool() = default;

	GP2DescriptorPool(const GP2DescriptorPool& other) = delete;
	GP2DescriptorPool(GP2DescriptorPool&& other) noexcept = default;
	GP2DescriptorPool& operator=(const GP2DescriptorPool& other) = delete;
	GP2DescriptorPool& operator=(GP2DescriptorPool&& other) noexcept = default;

	void DestroyPool(const VkDevice& device) const;

	void CreateDescriptorSets(const VkDevice& device, VkDescriptorSetLayout descriptorSetLayout, std::initializer_list<VkBuffer> buffers, const
		GP2Image& textureMap, const GP2Image& normalMap, const GP2Image& specularMap = {}, const GP2Image& glossMap = { });

	void BindDescriptorSet(VkCommandBuffer buffer, VkPipelineLayout layout, size_t index) const;

private:
	size_t m_Count;
	VkDescriptorPool m_DescriptorPool;
	std::vector<VkDescriptorSet> m_DescriptorSets;
};
