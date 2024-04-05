#pragma once
#include "GP2CommandBuffer.h"



struct QueueFamilyIndices;


class GP2CommandPool
{
public:
	GP2CommandPool();
	~GP2CommandPool() = default;

	void Initialize(const VkDevice& device, const QueueFamilyIndices& queue);
	void Destroy();
	GP2CommandBuffer CreateCommandBuffer() const;

	VkCommandPool GetVkCommandPool() const { return m_CommandPool; };

private:
	VkCommandPool m_CommandPool;
	VkDevice m_VkDevice;
};
