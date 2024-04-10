#include "GP2Scene.h"

#include "vulkanbase/VulkanBase.h"
#include "GP2CircleMesh.h"
#include "GP2RoundedRectangleMesh.h"

GP2Scene::GP2Scene()
{
	GP2CircleMesh circleMesh{ glm::vec2{0.25f, -0.5f}, 0.35f, 0.25f, 32 };
	m_Meshes.push_back(std::move(circleMesh));
	GP2RectangleMesh rectangleMesh{ glm::vec2{-0.5f, -0.5f}, 0.65f, 0.45f };
	m_Meshes.push_back(std::move(rectangleMesh));
	GP2RoundedRectangleMesh roundedRectangleMesh{ glm::vec2{0.25f, 0.65f}, 0.75f, 0.5f, 0.25f };
	m_Meshes.push_back(std::move(roundedRectangleMesh));
}

void GP2Scene::Initialize(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice, const GP2CommandPool& commandPool, const VkQueue& graphicsQueue)
{
	for (GP2Mesh& mesh : m_Meshes)
	{
		mesh.Initialize(vkDevice, vkPhysicalDevice, commandPool, graphicsQueue);
	}
}

void GP2Scene::Draw(const VkCommandBuffer& vkCommandBuffer)
{
	for (GP2Mesh& mesh : m_Meshes)
	{
		mesh.Draw(vkCommandBuffer);
	}
}
void GP2Scene::Cleanup(const VkDevice& vkDevice)
{
	for (GP2Mesh& mesh : m_Meshes)
	{
		mesh.Destroy(vkDevice);
	}
}