#pragma once

// ADD FOR FIXING WINDOW CPP max and min BUG WITH VULKAN 
#define NOMINMAX   
///////////////////////////////////////////////////// 

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <stdexcept>
#include <vector>
#include <cstring>

#include <cstdint> 
#include <cstddef>
#include <limits> 
#include <algorithm>
#include <glm/gtc/constants.hpp>


#include "Utilities.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

class VulkanRenderer
{
public:
	VulkanRenderer();

	int init(GLFWwindow* newWindow);
	void cleanup();
	void drawFrame();
	// -- Waiting
	void deviceWaitIdle();
	// -- Setter Functions
	void setFramebufferResized(bool value) {this -> framebufferResized = value;}
	void setAngle(int newAngle) {
		angle = newAngle;
	}
	
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<VulkanRenderer*>(glfwGetWindowUserPointer(window));
		app->setFramebufferResized(true);
	}

private:
	GLFWwindow* window;
	
	// New validation layer	
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> deviceExtensions = {
    	VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	// std::vector<VkDynamicState> dynamicStates = {
	// 	VK_DYNAMIC_STATE_VIEWPORT,
	// 	VK_DYNAMIC_STATE_LINE_WIDTH
	// };

	const std::vector<Vertex> vertices = {
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

	const std::vector<uint16_t> indices = {
    	0, 1, 2, 2, 3, 0
	};
	

	#ifdef NDEBUG
		const bool enableValidationLayers = false;
	#else
		const bool enableValidationLayers = true;
	#endif

	// Vulkan Components
	VkInstance instance;
	struct {
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
	} mainDevice;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;

	VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;

	// Pipeline
	VkShaderModule createShaderModule(const std::vector<char>& code);
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkRenderPass renderPass;

	// Drawing
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkCommandPool commandPool;

	// Before: Frames in flight
	// VkCommandBuffer commandBuffer;
	// VkSemaphore imageAvailableSemaphore;
	// VkSemaphore renderFinishedSemaphore;
	// VkFence inFlightFence;

	// Frames in flight -> array of commandBuffers
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	bool framebufferResized = false;
	uint32_t currentFrame = 0;

	// Vertex Buffer
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	
	// Motion
	uint32_t frame = 0;
	// const float motionSpeed = 0.002f;

	// Game Object
	std::vector<GameObject> gameObjects;
	int angle = 0;

	// Vulkan Functions
	// - Create Functions
	void createInstance();
	void createLogicalDevice();
	void createSurface();	
	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createGraphicsPipeline();
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffer();
	void createSyncObjects();
	void createVertexBuffer();
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void createIndexBuffer();


	// - Recreate Function
	void recreateSwapChain();

	// - Get Functions
	void getPhysicalDevice();
	void getGameObject();

	// - Support Functions
	// -- Checker Functions
	bool checkInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
	bool checkDeviceSuitable(VkPhysicalDevice device);
	bool checkValidationLayerSupport();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	// -- Getter & Finder Functions
	QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device); // Get SC support query
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	// -- Choosing Functions
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	// -- Copying Function
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	// -- Recording Funtion
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	
	// -- Cleanup SwapChain
	void cleanupSwapChain();

};

