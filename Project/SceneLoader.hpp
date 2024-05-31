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

void VulkanBase::LoadScene(const std::string& jsonPath, GP2Scene& scene)
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

			if (temp.type == "Cube")
			{
				const float width{ obj["width"] };
				const float height{ obj["height"] };
				scene.AddMesh(GP2CubeMesh{ temp.position, width, height });
				continue;
			}

			if (temp.type == "Sphere")
			{
				const float radius{ obj["radius"] };
				scene.AddMesh(GP2SphereMesh{ temp.position, radius });
				continue;
			}

			if (temp.type == "Obj")
			{
				const std::string path{ obj["path"] };

				GP23DMesh meshToAdd{ path, temp.position };
				meshToAdd.SetGlossMap();

				scene.AddMesh(GP23DMesh{ path, temp.position });
			}
		}

	}
	catch (const nlohmann::json::parse_error& e)
	{
		std::cout << e.what();
	}
}