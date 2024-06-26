#pragma once

#include <vector>

#include "GP2Camera.h"
#include "GP2Mesh.h"


class GP2Scene
{
public:
	GP2Scene() = default;
	~GP2Scene() = default;

	GP2Scene(const GP2Scene& other) = delete;
	GP2Scene(GP2Scene&& other) noexcept = delete;
	GP2Scene& operator=(const GP2Scene& other) = delete;
	GP2Scene& operator=(GP2Scene&& other) noexcept = delete;

	void Initialize(const VkDevice& vkDevice, const VkPhysicalDevice& vkPhysicalDevice, const GP2CommandPool& commandPool, const VkQueue& graphicsQueue);
	void Cleanup(const VkDevice& vkDevice) const;

	void Draw(const VkCommandBuffer& vkCommandBuffer, const VkPipelineLayout pipelineLayout, const VkExtent2D& swapChainExtent, const
	          GP2Camera& camera) const;
	void Update(const GP2Camera& camera, const bool useNormalMap = true, const bool useDiffuseMap = true, const bool useGlossMap = true, const bool
	            useSpecularMap = true);

	void AddMesh(GP2Mesh& mesh);

private:
	std::vector<GP2Mesh> m_Meshes{};
};