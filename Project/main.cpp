#include "vulkanbase/VulkanBase.h"


void PrintInstructions()
{
	std::cout << "Welcome :\n";
	std::cout << "Press these keys to toggle the different maps. (only works for the meshes of the diffuse specular piepline)\n";
	std::cout << "F3 : Toggle Diffuse Map\n";
	std::cout << "F4 : Toggle Normal Map\n";
	std::cout << "F5 : Toggle Specular Map\n";
	std::cout << "F6 : Toggle Gloss Map\n";
}

int main() {
	// DISABLE_LAYER_AMD_SWITCHABLE_GRAPHICS_1 = 1
	//DISABLE_LAYER_NV_OPTIMUS_1 = 1
	//_putenv_s("DISABLE_LAYER_AMD_SWITCHABLE_GRAPHICS_1", "1");
	//_putenv_s("DISABLE_LAYER_NV_OPTIMUS_1", "1");
	VulkanBase app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}