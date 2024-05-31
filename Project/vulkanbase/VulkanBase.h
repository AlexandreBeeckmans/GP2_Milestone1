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
	void LoadScene(const std::string& jsonPath, GP2Scene& scene);

	void initVulkan()
	{
		//GP2CircleMesh circleMesh{ glm::vec2{0.8f, -0.8f}, 0.2f, 0.25f, 32 };
		//m_2DScene.AddMesh(circleMesh);
		//GP2RectangleMesh rectangleMesh{ glm::vec2{-0.75f, -0.75f}, 0.65f, 0.45f };
		//m_2DScene.AddMesh(rectangleMesh);
		//GP2RoundedRectangleMesh roundedRectangleMesh{ glm::vec2{-0.75, 0.75f}, 0.75f, 0.5f, 0.25f };
		//m_2DScene.AddMesh(roundedRectangleMesh);

		//GP2CubeMesh cubeMesh{ glm::vec3{25.5f, -0.5f, -1.0f}, 5.0f, 5.0f };
		//GP2SphereMesh sphereMesh{ glm::vec3{15.0f,0.0f,0.0f}, 5.0f };
		//m_3DScene.AddMesh(cubeMesh);
		//m_3DScene.AddMesh(sphereMesh);

		
		


		//GP2SphereMesh pbrSphereMesh{ glm::vec3{0.0f,10.0f,0.0f}, 5.0f };
		//GP23DMesh stormtrooperMesh{ "resources/stormtrooper.obj", glm::vec3{0,0, -35} };
		//m_FloorPBRScene.AddMesh(pbrSphereMesh);
		//m_FloorPBRScene.AddMesh(stormtrooperMesh);

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

		/*m_TextureImage.Initialize(device, physicalDevice, m_CommandPool, graphicsQueue, "textures/vehicle_diffuse.png");
		m_NormalImage.Initialize(device, physicalDevice, m_CommandPool, graphicsQueue, "textures/vehicle_normal.png");
		m_SpecularImage.Initialize(device, physicalDevice, m_CommandPool, graphicsQueue, "textures/vehicle_specular.png");
		m_GlossImage.Initialize(device, physicalDevice, m_CommandPool, graphicsQueue, "textures/vehicle_gloss.png");*/

		//m_2DPipeline.Initialize(device, physicalDevice, m_CommandPool, graphicsQueue, m_RenderPass, &m_CommandBuffer, m_TextureImage, m_NormalImage, m_SpecularImage, m_GlossImage);
		//m_3DPipeline.Initialize(device, physicalDevice, m_CommandPool, graphicsQueue, m_RenderPass, &m_CommandBuffer, m_TextureImage, m_NormalImage, m_SpecularImage, m_GlossImage);
		LoadScene("scenes/PBRScene.json", m_PBRScene);
		m_PBRPipeline.Initialize(device, physicalDevice, m_CommandPool, graphicsQueue, m_RenderPass, &m_CommandBuffer, m_TextureImage, m_NormalImage, m_SpecularImage, m_GlossImage);
		

		//m_FloorTextureImage.Initialize(device, physicalDevice, m_CommandPool, graphicsQueue, "textures/floor_diffuse.jpg");
		//m_FloorNormalImage.Initialize(device, physicalDevice, m_CommandPool, graphicsQueue, "textures/floor_normal.jpg");
		//m_FloorSpecularImage.Initialize(device, physicalDevice, m_CommandPool, graphicsQueue, "textures/floor_specular.jpg");
		//m_FloorGlossImage.Initialize(device, physicalDevice, m_CommandPool, graphicsQueue, "textures/floor_gloss.jpg");
		//m_FloorPBRPipeline.Initialize(device, physicalDevice, m_CommandPool, graphicsQueue, m_RenderPass, &m_CommandBuffer);


		// week 06
		createSyncObjects();
	}

	void mainLoop() {
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();

			m_Camera.Update();
			m_3DScene.Update(m_Camera);
			m_PBRScene.Update(m_Camera);
			m_FloorPBRScene.Update(m_Camera);

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
		m_FloorPBRPipeline.Cleanup(device);

		for (auto framebuffer : m_SwapChainFramebuffers) {
			vkDestroyFramebuffer(device, framebuffer, nullptr);
		}
		vkDestroyRenderPass(device, m_RenderPass, nullptr);

		m_TextureImage.Destroy(device);
		m_NormalImage.Destroy(device);
		m_SpecularImage.Destroy(device);
		m_GlossImage.Destroy(device);

		m_FloorTextureImage.Destroy(device);
		m_FloorNormalImage.Destroy(device);
		m_FloorSpecularImage.Destroy(device);
		m_FloorGlossImage.Destroy(device);

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
	GP2Scene m_FloorPBRScene{};

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

	GP2Pipeline m_FloorPBRPipeline
	{
		"shaders/PBRShader.vert.spv",
		"shaders/PBRShader.frag.spv",
		& m_FloorPBRScene
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

	GP2Image m_TextureImage{};
	GP2Image m_NormalImage{};
	GP2Image m_SpecularImage{};
	GP2Image m_GlossImage{};

	GP2Image m_FloorTextureImage{};
	GP2Image m_FloorNormalImage{};
	GP2Image m_FloorSpecularImage{};
	GP2Image m_FloorGlossImage{};
};