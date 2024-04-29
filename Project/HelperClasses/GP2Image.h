#pragma once
#include "GP2CommandPool.h"
#include "vulkanbase/VulkanUtil.h"

class GP2Image final
{

public:
	void Initialize(const VkDevice& device, const VkPhysicalDevice& physicalDevice, const GP2CommandPool& commandPool, const VkQueue&
	                graphicsQueue, const std::string texturePath);

	void Destroy(const VkDevice& device);

	VkImageView GetImageView() const { return m_TextureImageView; }
	VkSampler GetSampler() const { return m_TextureSampler; }
private:
	void CreateTextureImage(const std::string& texturePath, const VkDevice& device, const VkPhysicalDevice& physicalDevice, const GP2CommandPool&
	                        commandPool, const VkQueue& graphicsQueue);
	void TransitionImageLayout(const VkDevice& device, const GP2CommandPool& commandPool, const VkQueue& graphicsQueue, VkImage image, VkFormat format, VkImageLayout
	                           oldLayout, VkImageLayout
	                           newLayout);
	void CopyBufferToImage(const VkDevice& device, const GP2CommandPool& commandPool, const VkQueue& graphicsQueue, VkBuffer buffer, VkImage image, uint32_t
	                       width, uint32_t height);
	static VkCommandBuffer BeginSingleTimeCommands(const VkDevice& device, const GP2CommandPool& commandPool);
	static void EndSingleTimeCommands(const VkDevice& device, const VkCommandBuffer& commandBuffer, const GP2CommandPool& commandPool, const VkQueue&
	                           graphicsQueue);
	void CreateTextureImageView(const VkDevice& device);
	void CreateImageSampler(const VkDevice& device, const VkPhysicalDevice& physicalDevice);

	VkImage m_TextureImage{};
	VkDeviceMemory m_TextureImageMemory{};
	VkImageView m_TextureImageView{};
	VkSampler m_TextureSampler{};
};
