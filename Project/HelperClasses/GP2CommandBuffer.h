#pragma once
#include "vulkan/vulkan_core.h"

class GP2CommandBuffer final
{
public:
	GP2CommandBuffer();
	~GP2CommandBuffer() = default;

	GP2CommandBuffer(const GP2CommandBuffer& other) = delete;
	GP2CommandBuffer(GP2CommandBuffer&& other) noexcept = default;
	GP2CommandBuffer& operator=(const GP2CommandBuffer& other) = delete;
	GP2CommandBuffer& operator=(GP2CommandBuffer&& other) noexcept = default;

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