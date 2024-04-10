#pragma once
#include <vulkan/vulkan_core.h>
#include "GP2Shader.h"
#include "GP2Scene.h"

class GP2Pipeline
{
public:
	GP2Pipeline();

	void Initialize();
	void Record();

private:
	void DrawScene();

	VkPipeline m_Pipeline;
	GP2Shader m_Shader;
	GP2CommandBuffer m_Buffer;
	GP2Scene m_Scene;
};