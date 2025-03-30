#pragma once

#ifndef INIT_OPERAIONS_VKCORE
#define INIT_OPERAIONS_VKCORE

#include "VK.h"



//Vulkan related Macro define
#define VK_USE_PLATFORM_WIN32_KHR		//String Macro    

//Vulkan related Header files
#include<vulkan/vulkan.h>

//Vulkan related libraries
#pragma comment(lib,"vulkan-1.lib")




//Vulkan related Global variables

struct VkCore
{
	//Vulkan Instance
	VkInstance vkInstance;

	//Instance extension related variables
	uint32_t enabledInstanceExtensionCount;

	//VK_KHR_SURFACE_EXTENSION_NAME and VK_KHR_WIN32_SURFACE_EXTENSION_NAME
	const char* enabledInstanceExtensionNames_array[2];

	//Vulkan presentation object
	VkSurfaceKHR vkSurfaceKHR;

	//Vulkan Physical device related
	VkPhysicalDevice vkPhysicalDevice_Selected;
	uint32_t graphicsQueueFamilyIndex_Selected;
	VkPhysicalDeviceMemoryProperties vkPhysicalDeviceMemoryProperties;

	uint32_t physicalDeviceCount;
	VkPhysicalDevice* vkPhysicalDevice_array;

	////////For Device Extension Printing
	//Device extension related variables
	uint32_t enabledDeviceExtensionCount;

	//VK_KHR_SWAPCHAIN_EXTENSION_NAME
	const char* enabledDeviceExtensionNames_array[1];

	//Vulkan Device
	VkDevice vkDevice;

	//Device Queue
	VkQueue vkQueue;

	//Color Format and Color Space
	VkFormat vkFormat_color;
	VkColorSpaceKHR vkColorSpacrKHR;

	//Presentation Mode
	VkPresentModeKHR vkPresentModeKHR;

	//SwapChain releted global variables
	VkSwapchainKHR vkSwapchainKHR;
	VkExtent2D vkExtent2D_Swapchain;

	//Swapchain Image and Swapchain ImageView related variables
	uint32_t swapchainImageCount;
	VkImage* swapchainImage_array;
	VkImageView* swapchainImageView_array;

	//Command Pool variables
	VkCommandPool vkCommandPool;


}VkCore;



// Function declarations Prototypes
VkResult fillInstanceExtensionNames();

VkResult  createVulkanInstance();

VkResult getSupportedSurface();

VkResult getPhysicalDevice(void);

VkResult printVKInfo(void);

void getDeviceQueue(void);

VkResult fillDeviceExtensionNames(void);

VkResult createVulkanDevice(void);

VkResult getPhysicalDeviceSurfaceFormatAndColorSpace(void);

VkResult getPhysicalDevicePresentMode(void);


#endif