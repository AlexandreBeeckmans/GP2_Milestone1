#pragma once
#include <string>
#include"HelperClasses/GP2Scene.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include"vulkanbase/VulkanBase.h"

struct LoadedMesh
{
	glm::vec3 position{};
	std::string type{};
};

void VulkanBase::LoadScene(const std::string& jsonPath, GP2Scene& scene, const std::string& fragmentShaderPath, const std::string& vertexShaderPath)
{
	std::ifstream inputFile(jsonPath);
	if (!inputFile.is_open())
	{
		return;
	}


	nlohmann::json doc;
	try
	{
		doc = nlohmann::json::parse(inputFile);

		for (const auto& obj : doc)
		{
			LoadedMesh temp{};
			temp.position.x = obj["pos"]["x"];
			temp.position.y = obj["pos"]["y"];
			temp.position.z = obj["pos"]["z"];
			temp.type = obj["type"];

			GP2Mesh meshToAdd{};

			if (temp.type == "Cube")
			{
				const float width{ obj["width"] };
				const float height{ obj["height"] };
				meshToAdd = GP2CubeMesh{ temp.position, width, height };
			}

			if (temp.type == "Sphere")
			{
				const float radius{ obj["radius"] };
				meshToAdd = GP2SphereMesh{ temp.position, radius };
			}

			if (temp.type == "Obj")
			{
				const std::string path{ obj["path"] };

				meshToAdd = GP23DMesh { path, temp.position };
			}

			
			meshToAdd.SetTextureMap(device, physicalDevice, m_CommandPool, graphicsQueue, obj["textureMap"]);
			meshToAdd.SetNormalMap(device, physicalDevice, m_CommandPool, graphicsQueue, obj["normalMap"]);
			meshToAdd.SetSpecularMap(device, physicalDevice, m_CommandPool, graphicsQueue, obj["specularMap"]);
			meshToAdd.SetGlossMap(device, physicalDevice, m_CommandPool, graphicsQueue,obj["glossMap"]);
			meshToAdd.InitShader(device, physicalDevice, m_CommandPool, graphicsQueue, vertexShaderPath, fragmentShaderPath);


			scene.AddMesh(meshToAdd);
		}

	}
	catch (const nlohmann::json::parse_error& e)
	{
		std::cout << e.what();
	}
}
