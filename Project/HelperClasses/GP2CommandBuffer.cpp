#include "GP2CommandBuffer.h"
#include <stdexcept>


GP2CommandBuffer::GP2CommandBuffer() :
	m_CommandBuffer{}
{}

void GP2CommandBuffer::Reset() const
{
	vkResetCommandBuffer(m_CommandBuffer, /*VkCommandBufferResetFlagBits*/ 0);
}

void GP2CommandBuffer::BeginRecording(const VkCommandBufferUsageFlags& usage) const
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = usage; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	if (vkBeginCommandBuffer(m_CommandBuffer, &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}
}

void GP2CommandBuffer::EndRecording() const
{
	if (vkEndCommandBuffer(m_CommandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}
}

void GP2CommandBuffer::Submit(VkSubmitInfo& info) const
{
	info.commandBufferCount = 1;
	info.pCommandBuffers = &m_CommandBuffer;
}

