#include "GP2Scene.h"

#include "vulkanbase/VulkanBase.h"
#include "GP23DMesh.h"

void GP2Scene::Initialize(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice, const GP2CommandPool& commandPool, const VkQueue& graphicsQueue)
{
	for (GP2Mesh& mesh : m_Meshes)
	{
		mesh.Initialize(vkDevice, vkPhysicalDevice, commandPool, graphicsQueue);
	}
}

void GP2Scene::Draw(const VkCommandBuffer& vkCommandBuffer) const
{
	for (const GP2Mesh& mesh : m_Meshes)
	{
		mesh.Draw(vkCommandBuffer);
	}
}

void GP2Scene::AddMesh(GP2Mesh& mesh)
{
	m_Meshes.push_back(std::move(mesh));
}
void GP2Scene::Cleanup(const VkDevice& vkDevice) const
{
	for (const GP2Mesh& mesh : m_Meshes)
	{
		mesh.Destroy(vkDevice);
	}
}
