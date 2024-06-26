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

void GP2Scene::Draw(const VkCommandBuffer& vkCommandBuffer, const VkPipelineLayout pipelineLayout, const VkExtent2D& swapChainExtent, const GP2Camera& camera) const
{
	for (const GP2Mesh& mesh : m_Meshes)
	{
		mesh.Draw(vkCommandBuffer, pipelineLayout, swapChainExtent, camera);
	}
}

void GP2Scene::Update(const GP2Camera& camera, const bool useNormalMap, const bool useDiffuseMap, const bool useGlossMap, const bool useSpecularMap)
{
	for (GP2Mesh& mesh : m_Meshes)
	{
		mesh.Update(camera.GetPosition(), useNormalMap, useDiffuseMap, useGlossMap, useSpecularMap);
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
