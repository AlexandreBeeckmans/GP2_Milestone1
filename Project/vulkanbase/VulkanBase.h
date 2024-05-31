#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION



#include "VulkanUtil.h"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <optional>


#include "HelperClasses/GP2Shader.h"
#include "HelperClasses/GP2CommandPool.h"
#include "HelperClasses/GP2CommandBuffer.h"

#include "HelperClasses/GP2Scene.h"
#include <HelperClasses/GP2Pipeline.h>

#include "HelperClasses/GP23DMesh.h"
#include "HelperClasses/GP2Camera.h"
#include "HelperClasses/GP2CircleMesh.h"
#include "HelperClasses/GP2CubeMesh.h"
#include "HelperClasses/GP2Image.h"
#include "HelperClasses/GP2RoundedRectangleMesh.h"
#include "HelperClasses/GP2SphereMesh.h"

constexpr int MAX_FRAMES_IN_FLIGHT = 3;


const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() const
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class VulkanBase
{
public:
	VulkanBase() = default;

	void KeyEvent(int key, int scancode, int action, int mods);

	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

	static void CreateImage(const VkDevice& device, const VkPhysicalDevice& physicalDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(device, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = GP2DataBuffer::FindMemoryType(memRequirements.memoryTypeBits, properties, physicalDevice);

		if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(device, image, imageMemory, 0);
	}
	static VkImageView CreateImageView(const VkDevice& device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;
		if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture image view!");
		}

		return imageView;
	}


private:
	void LoadScene(const std::string& jsonPath, GP2Scene& scene, const std::string& fragmentShaderPath, const std::string&
	               vertexShaderPath);

	void initVulkan()
	{

		// week 06
		createInstance();
		setupDebugMessenger();
		createSurface();

		// week 05
		pickPhysicalDevice();
		createLogicalDevice();

		// week 04 
		createSwapChain();
		createImageViews();

		CreateRenderPass();

		CreateDepthResources();
		CreateFrameBuffers();

		m_CommandPool.Initialize(device, findQueueFamilies(physicalDevice));
		m_CommandBuffer = m_CommandPool.CreateCommandBuffer();

		LoadScene("scenes/2DScene.json", m_2DScene, m_2DPipeline.GetFragmentShaderPath(), m_2DPipeline.GetVertexShaderPath());
		m_2DPipeline.Initialize(device, physicalDevice, m_CommandPool, graphicsQueue, m_RenderPass, &m_CommandBuffer);


		LoadScene("scenes/3DScene.json", m_3DScene, m_3DPipeline.GetFragmentShaderPath(), m_3DPipeline.GetVertexShaderPath());
		m_3DPipeline.Initialize(device, physicalDevice, m_CommandPool, graphicsQueue, m_RenderPass, &m_CommandBuffer);

		LoadScene("scenes/PBRScene.json", m_PBRScene, m_PBRPipeline.GetFragmentShaderPath(), m_PBRPipeline.GetVertexShaderPath());
		m_PBRPipeline.Initialize(device, physicalDevice, m_CommandPool, graphicsQueue, m_RenderPass, &m_CommandBuffer);


		// week 06
		createSyncObjects();
	}

	void mainLoop() {
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();

			m_Camera.Update();


			m_3DScene.Update(m_Camera);
			m_PBRScene.Update(m_Camera, m_UseNormalMap, m_UseDiffuseMap, m_UseGlossinessMap, m_UseSpecularMap);

			// week 06
			drawFrame();
		}
		vkDeviceWaitIdle(device);
	}

	void cleanup() 
	{

		vkDestroyImageView(device, m_DepthImageView, nullptr);
		vkDestroyImage(device, m_DepthImage, nullptr);
		vkFreeMemory(device, m_DepthImageMemory, nullptr);

		vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
		vkDestroyFence(device, inFlightFence, nullptr);
		
		m_2DPipeline.Cleanup(device);
		m_3DPipeline.Cleanup(device);
		m_PBRPipeline.Cleanup(device);

		for (auto framebuffer : m_SwapChainFramebuffers) {
			vkDestroyFramebuffer(device, framebuffer, nullptr);
		}
		vkDestroyRenderPass(device, m_RenderPass, nullptr);

		m_CommandPool.Destroy();

		
		for (auto imageView : swapChainImageViews) {
			vkDestroyImageView(device, imageView, nullptr);
		}

		if (enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		}
		vkDestroySwapchainKHR(device, swapChain, nullptr);
		vkDestroyDevice(device, nullptr);

		vkDestroySurfaceKHR(instance, surface, nullptr);
		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);
		glfwTerminate();
	}

	

	

	void createSurface() {
		if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

	// Week 01: 
	// Actual window
	// simple fragment + vertex shader creation functions
	// These 5 functions should be refactored into a separate C++ class
	// with the correct internal state.

	GLFWwindow* window;
	void initWindow();

	// Week 02
	// Queue families
	// CommandBuffer concept
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	void CreateRenderPass();
	void BeginRenderPass(const GP2CommandBuffer& buffer, VkFramebuffer currentBuffer, VkExtent2D extent);
	void EndRenderPass(const GP2CommandBuffer& buffer);
	void CreateFrameBuffers();


	// Week 04
	// Swap chain and image view support

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkFramebuffer> m_SwapChainFramebuffers;

	std::vector<VkImageView> swapChainImageViews;

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	void createSwapChain();
	void createImageViews();

	// Week 05 
	// Logical and physical device

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	
	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	void createLogicalDevice();

	// Week 06
	// Main initialization

	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkDevice device = VK_NULL_HANDLE;
	VkSurfaceKHR surface;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkFence inFlightFence;

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void setupDebugMessenger();
	std::vector<const char*> getRequiredExtensions();
	static bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	void createInstance();

	void createSyncObjects();
	void drawFrame();


	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}
	uint32_t m_CurrentFrame = 0;


	GP2Scene m_2DScene{};
	GP2Scene m_3DScene{};
	GP2Scene m_PBRScene{};

	GP2Pipeline m_2DPipeline
	{
		"shaders/shader.vert.spv",
		"shaders/shader.frag.spv",
		&m_2DScene
	};

	GP2Pipeline m_3DPipeline
	{
		"shaders/3DShader.vert.spv",
		"shaders/3DShader.frag.spv",
		&m_3DScene
	};

	GP2Pipeline m_PBRPipeline
	{
		"shaders/PBRShader.vert.spv",
		"shaders/PBRShader.frag.spv",
		&m_PBRScene
	};

	VkRenderPass m_RenderPass{};
	GP2CommandPool m_CommandPool{};
	GP2CommandBuffer m_CommandBuffer{};

	VkImage m_DepthImage{};
	VkDeviceMemory m_DepthImageMemory{};
	VkImageView m_DepthImageView{};





	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const
	{
		for(VkFormat format : candidates)
		{
			VkFormatProperties properties{};
			vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &properties);

			if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features)
			{
				return format;
			}

			if(tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features)
			{
				return format;
			}

			throw std::runtime_error("failed to find supported format");
		}
		return{};
	}
	VkFormat FindDepthFormat() const
	{
		return FindSupportedFormat
		(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	static bool HasStencilComponent(VkFormat format)
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	void CreateDepthResources()
	{
		VkFormat depthFormat = FindDepthFormat();

		CreateImage
		(
			device,
			physicalDevice,
			swapChainExtent.width, 
			swapChainExtent.height, 
			depthFormat,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_DepthImage,
			m_DepthImageMemory
		);

		m_DepthImageView = CreateImageView(device, m_DepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
	}

	GP2Camera m_Camera{ {0,0,-100}, 30, WIDTH, HEIGHT };


	bool m_UseNormalMap{ true };
	bool m_UseGlossinessMap{ true };
	bool m_UseSpecularMap{ true };
	bool m_UseDiffuseMap{ true };
};



//Inputs
inline void VulkanBase::KeyEvent(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
	{
		m_UseDiffuseMap = !m_UseDiffuseMap;
		return;
	}
	if (key == GLFW_KEY_F4 && action == GLFW_PRESS)
	{
		m_UseNormalMap = !m_UseNormalMap;
		return;
	}
	if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
	{
		m_UseSpecularMap = !m_UseSpecularMap;
		return;
	}
	if (key == GLFW_KEY_F6 && action == GLFW_PRESS)
	{
		m_UseGlossinessMap = !m_UseGlossinessMap;
		return;
	}
}