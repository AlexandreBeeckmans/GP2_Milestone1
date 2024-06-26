#include "vulkanbase/VulkanBase.h"
#include <stb_image.h>

void VulkanBase::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

	glfwSetWindowUserPointer(window, this);

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			void* pUser = glfwGetWindowUserPointer(window);
			VulkanBase* vBase = static_cast<VulkanBase*>(pUser);
			vBase->m_Camera.KeyEvent(key, scancode, action, mods);
			vBase->KeyEvent(key, scancode, action, mods);
		});

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
		{
			void* pUser = glfwGetWindowUserPointer(window);
			VulkanBase* vBase = static_cast<VulkanBase*>(pUser);
			vBase->m_Camera.MouseMove(window, xpos, ypos);
		});
	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
		{
			void* pUser = glfwGetWindowUserPointer(window);
			VulkanBase* vBase = static_cast<VulkanBase*>(pUser);
			vBase->m_Camera.MouseEvent(window, button, action, mods);
		});
}
