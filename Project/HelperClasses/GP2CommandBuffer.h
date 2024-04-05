#pragma once
#include "vulkan/vulkan_core.h"

class GP2CommandBuffer
{
public:
	GP2CommandBuffer();
	~GP2CommandBuffer() = default;

	void SetVkCommandBuffer(VkCommandBuffer buffer)
	{
		m_CommandBuffer = buffer;
	}

	VkCommandBuffer GetVkCommandBuffer()const
	{
		return m_CommandBuffer;
	}

	const VkCommandBuffer* GetpVkCommandBuffer()const
	{
		return &(m_CommandBuffer);
	}

	void Reset()const;
	void BeginRecording(const VkCommandBufferUsageFlags& usage = 0)const;
	void EndRecording()const;

	void Submit(VkSubmitInfo& info)const;
private:
	VkCommandBuffer m_CommandBuffer;
};