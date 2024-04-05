#include "GP2Scene.h"

#include "vulkanbase/VulkanBase.h"

GP2Scene::GP2Scene()
{

}

void GP2Scene::Initialize(const VkPhysicalDevice& vkPhysicalDevice, const VkDevice& vkDevice)
{
	GP2Mesh baseMesh{};
	baseMesh.Initialize(vkDevice, vkPhysicalDevice);
	m_Meshes.push_back(baseMesh);

	/*GP2RectangleMesh rectangleMesh{ glm::vec2{-0.5f, -0.5f}, 0.65f, 0.45f };
	rectangleMesh.Initialize(vkPhysicalDevice, vkDevice);
	m_Meshes.push_back(std::move(rectangleMesh));


	GP2CircleMesh circleMesh{ glm::vec2{0.25f, -0.5f}, 0.35f, 0.25f, 32 };
	circleMesh.Initialize(vkPhysicalDevice, vkDevice);
	m_Meshes.push_back(std::move(circleMesh));

	GP2RoundedRectangleMesh roundedRectangleMesh{ glm::vec2{0.25f, 0.65f}, 0.75f, 0.5f, 0.25f };
	roundedRectangleMesh.Initialize(vkPhysicalDevice, vkDevice);
	m_Meshes.push_back(std::move(roundedRectangleMesh));*/
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