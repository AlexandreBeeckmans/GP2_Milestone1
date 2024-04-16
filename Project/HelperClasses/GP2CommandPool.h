#pragma once
#include "GP2CommandBuffer.h"



struct QueueFamilyIndices;


class GP2CommandPool final
{
public:
	GP2CommandPool();
	~GP2CommandPool() = default;

	GP2CommandPool(const GP2CommandPool& other) = default;
	GP2CommandPool(GP2CommandPool&& other) noexcept = default;
	GP2CommandPool& operator=(const GP2CommandPool& other) = delete;
	GP2CommandPool& operator=(GP2CommandPool&& other) noexcept = default;

	void Initialize(const VkDevice& device, const QueueFamilyIndices& queue);
	void Destroy() const;
	GP2CommandBuffer CreateCommandBuffer() const;

	VkCommandPool GetVkCommandPool() const { return m_CommandPool; };

private:
	VkCommandPool m_CommandPool;
	VkDevice m_VkDevice;
};
