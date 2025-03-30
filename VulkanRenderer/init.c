#include<stdio.h>
#include"init.h"
FILE* gpFile = NULL;
//

int winWidth = WINDOW_WIDTH;
int winHeight = WINDOW_HEIGHT;

//////////////////////////////////////////////////////////////////////////////////// Definition of Vulkan related Functions ///////////////////////////////////////////////////////////////////////////////

void vulkanCore(void)
{
	gpFile = fopen("VulkanLog.txt", "w");
	if (gpFile == NULL)
	{
		MessageBox(NULL, TEXT("\nFailed to create log file. Exiting now..."), TEXT("File I/O Error"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "\nWinMain()->Program Started successfully.\n");
	}
}
VkResult createVulkanInstance(void)
{
	VkCore.vkInstance = VK_NULL_HANDLE;
	vulkanCore();
	// Function declarations
	VkResult fillInstanceExtensionNames();

	VkResult vkResult = VK_SUCCESS;

	// Code
	vkResult = fillInstanceExtensionNames();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVulkanInstance()->fillInstanceExtensionNames Function is Failed");
		return vkResult;
	}
	else {
		fprintf(gpFile, "createVulkanInstance()->fillInstanceExtensionNames Function is Succeded");
	}

	//Initialize struct VkApplicationInfo
	VkApplicationInfo vkApplicationInfo;
	memset((void*)&vkApplicationInfo, 0, sizeof(VkApplicationInfo));

	vkApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	vkApplicationInfo.pNext = NULL;
	vkApplicationInfo.pApplicationName = "ARTR";
	vkApplicationInfo.applicationVersion = 1;
	vkApplicationInfo.pEngineName = "ARTR";
	vkApplicationInfo.engineVersion = 1;
	vkApplicationInfo.apiVersion = VK_API_VERSION_1_4;

	//INITIALIZE STRUCT
	VkInstanceCreateInfo vkInstanceCreateInfo;
	memset((void*)&vkInstanceCreateInfo, 0, sizeof(VkInstanceCreateInfo));

	vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vkInstanceCreateInfo.pNext = NULL;
	vkInstanceCreateInfo.pApplicationInfo = &vkApplicationInfo;
	vkInstanceCreateInfo.enabledExtensionCount = VkCore.enabledInstanceExtensionCount;
	vkInstanceCreateInfo.ppEnabledExtensionNames = VkCore.enabledInstanceExtensionNames_array;

	//CALL create instance by calling vkInstance
	vkResult = vkCreateInstance(&vkInstanceCreateInfo, NULL, &VkCore.vkInstance);
	if (vkResult == VK_ERROR_INCOMPATIBLE_DRIVER)
	{
		fprintf(gpFile, "vkCreateInstance() failed due to incompatible driver %d\n", vkResult);
		return vkResult;
	}
	else if (vkResult == VK_ERROR_EXTENSION_NOT_PRESENT)
	{
		fprintf(gpFile, "vkCreateInstance() failed due to extension not present driver %d\n", vkResult);
		return vkResult;
	}
	else if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "vkCreateInstance() failed due to Unknown reason driver %d\n", vkResult);
		return vkResult;
	}
	else {
		fprintf(gpFile, "vkCreateInstance() succeded");
	}
	return vkResult;
}


VkResult fillInstanceExtensionNames(void)
{
	vulkanCore();
	VkCore.enabledInstanceExtensionCount = 0;

	// Variable declarations
	VkResult vkResult = VK_SUCCESS;

	//1.Find how many extensions are supported by vulkan driver of this version and keep it in local variable
	uint32_t instanceExtensionCount = 0;

	//First vulkan API
	vkResult = vkEnumerateInstanceExtensionProperties(NULL, &instanceExtensionCount, NULL);
	//NULL : Layer name supported by Vulkan,  &instanceextensioncount : count or size, NULL: Extension properties array(parameterised returm value)
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "First call to fillInstanceExtensionNames()->vkEnumerateInstanceExtensionProperties Function is Failed");
		return (vkResult);
	}
	else {
		fprintf(gpFile, "First call to fillInstanceExtensionNames()->vkEnumerateInstanceExtensionProperties Function is Succeded");
	}

	//2. Allocate and Fill VkExtensionProperties struct properties corresponding to above count  
	VkExtensionProperties* vkExtensionProperties_array = NULL;
	vkExtensionProperties_array = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * instanceExtensionCount);			//Allocate memory for VkExxtension
	//For the sake of bravety no malloc checking code


	vkResult = vkEnumerateInstanceExtensionProperties(NULL, &instanceExtensionCount, vkExtensionProperties_array);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "Second call to fillInstanceExtensionNames()->vkEnumerateInstanceExtensionProperties Function is Failed");
		return (vkResult);
	}
	else {
		fprintf(gpFile, "Second call to fillInstanceExtensionNames()->vkEnumerateInstanceExtensionProperties Function is Succeded");
	}


	//3. Fill and display a local string array of extension names obtained from vkExtensionProperties structure array
	char** instanceExtensionNames_array = NULL;
	instanceExtensionNames_array = (char**)malloc(sizeof(char*) * instanceExtensionCount);
	for (uint32_t i = 0; i < instanceExtensionCount; i++)
	{
		instanceExtensionNames_array[i] = (char*)malloc(sizeof(char) * strlen(vkExtensionProperties_array[i].extensionName) + 1);
		memcpy(instanceExtensionNames_array[i], vkExtensionProperties_array[i].extensionName, strlen(vkExtensionProperties_array[i].extensionName) + 1);
		fprintf(gpFile, "fillInstanceExtensionNames()->vulkanExtensionName = %s\n", instanceExtensionNames_array[i]);
	}

	//4. As not required here onward free the vkExtension properties array
	free(vkExtensionProperties_array);
	vkExtensionProperties_array = NULL;		//bhanda swachha 


	//5.Find weather above extensions contain our required 2 extensions
	VkBool32 vulkanSurfaceExtensionFound = VK_FALSE;
	VkBool32 win32SurfaceExtensionFound = VK_FALSE;

	for (uint32_t i = 0; i < instanceExtensionCount; i++)
	{
		if (strcmp(instanceExtensionNames_array[i], VK_KHR_SURFACE_EXTENSION_NAME) == 0)
		{
			vulkanSurfaceExtensionFound = VK_TRUE;
			VkCore.enabledInstanceExtensionNames_array[VkCore.enabledInstanceExtensionCount++] = VK_KHR_SURFACE_EXTENSION_NAME;

		}

		if (strcmp(instanceExtensionNames_array[i], VK_KHR_WIN32_SURFACE_EXTENSION_NAME) == 0)
		{
			win32SurfaceExtensionFound = VK_TRUE;
			VkCore.enabledInstanceExtensionNames_array[VkCore.enabledInstanceExtensionCount++] = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
		}
	}

	//6 As not needed henceforth free the local strings array
	for (uint32_t i = 0; i < instanceExtensionCount; i++)
	{
		free(instanceExtensionNames_array[i]);
	}
	free(instanceExtensionNames_array);


	//7.Display weather required instance extensions supported or not
	if (vulkanSurfaceExtensionFound == VK_FALSE)
	{
		vkResult = VK_ERROR_INITIALIZATION_FAILED;			//Return hardcoded failure
		fprintf(gpFile, "fillInstanceExtensionNames()->VK_KHR_SURFACE_EXTENSION_NAME not found");
		return (vkResult);
	}
	else {
		fprintf(gpFile, "fillInstanceExtensionNames()->VK_KHR_SURFACE_EXTENSION_NAME  is found");
	}


	if (win32SurfaceExtensionFound == VK_FALSE)
	{
		vkResult = VK_ERROR_INITIALIZATION_FAILED;			//Return hardcoded failure
		fprintf(gpFile, "fillInstanceExtensionNames()->VK_KHR_WIN32_SURFACE_EXTENSION_NAME not found");
		return (vkResult);
	}
	else
	{
		fprintf(gpFile, "fillInstanceExtensionNames()->VK_KHR_WIN32_SURFACE_EXTENSION_NAME  is found");
	}

	//8. Print only enabled extension names
	for (uint32_t i = 0; i < VkCore.enabledInstanceExtensionCount; i++)
	{
		fprintf(gpFile, "fillInstanceExtensionNames()->EnabledVulkanExtensionName = %s\n", VkCore.enabledInstanceExtensionNames_array[i]);
		return (vkResult);
	}

	return (vkResult);
}

////presentation surface

VkResult getSupportedSurface(void)
{
	VkCore.vkSurfaceKHR = VK_NULL_HANDLE;
	vulkanCore();

	// Variable declarations
	VkResult vkResult = VK_SUCCESS;

	VkWin32SurfaceCreateInfoKHR vkWin32SurfaceCreateInfoKHR;

	memset((void*)&vkWin32SurfaceCreateInfoKHR, 0, sizeof(VkWin32SurfaceCreateInfoKHR));

	vkWin32SurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	vkWin32SurfaceCreateInfoKHR.pNext = NULL;
	vkWin32SurfaceCreateInfoKHR.flags = 0;
	vkWin32SurfaceCreateInfoKHR.hinstance = (HINSTANCE)GetWindowLongPtr(ghwnd, GWLP_HINSTANCE);
	vkWin32SurfaceCreateInfoKHR.hwnd = ghwnd;

	vkResult = vkCreateWin32SurfaceKHR(VkCore.vkInstance, &vkWin32SurfaceCreateInfoKHR, NULL, &VkCore.vkSurfaceKHR);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "getSupportedSurface()->vkCreateWin32SurfaceKHR Function is Failed");
		return vkResult;
	}
	else {
		fprintf(gpFile, "getSupportedSurface()->vkCreateWin32SurfaceKHR Function is Succeded");
	}

	return vkResult;

}




///Physical Device
VkResult getPhysicalDevice()
{
	// Variable declarations
	VkResult vkResult = VK_SUCCESS;

	uint32_t physicalDeviceCount = 0;

	VkCore.vkPhysicalDevice_Selected = VK_NULL_HANDLE;
	VkCore.graphicsQueueFamilyIndex_Selected = UINT32_MAX;
	vulkanCore();

	vkResult = vkEnumeratePhysicalDevices(VkCore.vkInstance, &physicalDeviceCount, NULL);
	if (vkResult != VK_SUCCESS)
	{ 
		fprintf(gpFile, "getPhysicalDevice() - vkEnumeratePhysicalDevices() First call failed\n");
		return(vkResult);
	}
	else if (physicalDeviceCount == 0)
	{
		fprintf(gpFile, "getPhysicalDevice() - vkEnumeratePhysicalDevices resulted in 0 physicalDevices \n");
		return(vkResult);
	}
	else
	{
		fprintf(gpFile, "getPhysicalDevice() - vkEnumeratePhysicalDevices()  First call succeeded\n");
	}


	//2. Allocate
	VkPhysicalDevice* vkPhysicalDevice_array = NULL;

	vkPhysicalDevice_array = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * physicalDeviceCount);
	//error checking for malloc should be done

	//Step 4
	vkResult = vkEnumeratePhysicalDevices(VkCore.vkInstance, &physicalDeviceCount, vkPhysicalDevice_array);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "getPhysicalDevice() - vkEnumeratePhysicalDevices() Second call failed\n");
		return(vkResult);
	}
	else
	{
		fprintf(gpFile, "getPhysicalDevice() - vkEnumeratePhysicalDevices() Second call succeeded\n");
	}


	//5
	VkBool32 bFound = VK_FALSE;
	for (uint32_t i = 0; i < physicalDeviceCount; i++)
	{
		uint32_t queueCount = UINT32_MAX;

		//b
		vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice_array[i], &queueCount, NULL);

		//c
		VkQueueFamilyProperties* vkQueFamilyProperties_array = NULL;
		vkQueFamilyProperties_array = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * queueCount);


		vkResult = vkEnumeratePhysicalDevices(VkCore.vkInstance, &queueCount, vkQueFamilyProperties_array);


		//d
		vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice_array[i], &queueCount, vkQueFamilyProperties_array);


		VkBool32* isQueueSurfaceSupported_array = NULL;
		isQueueSurfaceSupported_array = (VkBool32*)malloc(sizeof(VkBool32) * queueCount);

		for (uint32_t j = 0; j < queueCount; j++)
		{
			vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice_array[i], j, VkCore.vkSurfaceKHR, &isQueueSurfaceSupported_array[j]);
		}

		for (uint32_t j = 0; j < queueCount; j++)
		{
			if (vkQueFamilyProperties_array[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				if (isQueueSurfaceSupported_array[j] == VK_TRUE)
				{
					VkCore.vkPhysicalDevice_Selected = vkPhysicalDevice_array[i];
					VkCore.graphicsQueueFamilyIndex_Selected = j;
					bFound = VK_TRUE;
					break;
				}
			}
		}

		if (isQueueSurfaceSupported_array)
		{
			free(isQueueSurfaceSupported_array);
			isQueueSurfaceSupported_array = NULL;
			fprintf(gpFile, "\ngetPhysicalDevice()-> isQueueSurfaceSupported_array Succeded to Free()");
		}
		if (vkQueFamilyProperties_array)
		{
			free(vkQueFamilyProperties_array);
			vkQueFamilyProperties_array = NULL;
			fprintf(gpFile, "\ngetPhysicalDevice()-> isQueueSurfaceSupported_array Succeded to Free()");
		}
		if (bFound == VK_TRUE)
		{
			break;
		}
	} // Main for loop


	if (bFound == VK_TRUE)
	{
		fprintf(gpFile, "getPhysicalDevice() is Succeeded to select the required physical device with graphics enabled\n");
		if (vkPhysicalDevice_array)
		{
			free(vkPhysicalDevice_array);
			vkPhysicalDevice_array = NULL;
			fprintf(gpFile, "getPhysicalDevice() Succeeded to free vkPhysicalDevice_array\n");
		}
	}
	else
	{
		fprintf(gpFile, "getPhysicalDevice() failed to select the required physical device with graphics enabled\n");
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		return(vkResult);
	}

	//7
	memset((void*)&VkCore.vkPhysicalDeviceMemoryProperties, 0, sizeof(VkPhysicalDeviceMemoryProperties));

	//8
	vkGetPhysicalDeviceMemoryProperties(VkCore.vkPhysicalDevice_Selected, &VkCore.vkPhysicalDeviceMemoryProperties);

	//9
	VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures;
	memset((void*)&vkPhysicalDeviceFeatures, 0, sizeof(VkPhysicalDeviceFeatures));

	vkGetPhysicalDeviceFeatures(VkCore.vkPhysicalDevice_Selected, &vkPhysicalDeviceFeatures);

	if (vkPhysicalDeviceFeatures.tessellationShader)
	{
		fprintf(gpFile, "\n getPhysicalDevice()-> vkPhysicalDeviceFeatures is supports tessellation shader");
	}
	else
	{
		fprintf(gpFile, "\n getPhysicalDevice()-> vkPhysicalDeviceFeatures is not supported tessellation shader");
	}
	if (vkPhysicalDeviceFeatures.geometryShader)
	{
		fprintf(gpFile, "\n getPhysicalDevice()-> vkPhysicalDeviceFeatures is supports geometryShader shader");
	}
	else
	{
		fprintf(gpFile, "\n getPhysicalDevice()-> vkPhysicalDeviceFeatures is not supportsB geometryShader shader");
	}
	return vkResult;
}


VkResult printVKInfo(void)
{
	// Variable declarations
	VkResult vkResult = VK_SUCCESS;

	VkCore.physicalDeviceCount = 0;
	VkCore.vkPhysicalDevice_array = NULL;

	vulkanCore();

	//Code
	fprintf(gpFile, "\n\n*********************************** Vulkan Information*********************************************\n\n");

	for (uint32_t i = 0; i < VkCore.physicalDeviceCount; i++)
	{
		VkPhysicalDeviceProperties vkPhysicalDeviceproperties;
		memset((void*)&vkPhysicalDeviceproperties, 0, sizeof(VkPhysicalDeviceProperties));
		vkGetPhysicalDeviceProperties(VkCore.vkPhysicalDevice_array[i], &vkPhysicalDeviceproperties);

		uint32_t majorVersion = VK_VERSION_MAJOR(vkPhysicalDeviceproperties.apiVersion);
		uint32_t minorVersion = VK_VERSION_MINOR(vkPhysicalDeviceproperties.apiVersion);
		uint32_t patchVersion = VK_VERSION_PATCH(vkPhysicalDeviceproperties.apiVersion);

		//API Version
		fprintf(gpFile, "\n API Version = %d.%d.%d\n", majorVersion, minorVersion, patchVersion);

		//Device Name
		fprintf(gpFile, "\n Device Name = %s\n", vkPhysicalDeviceproperties.deviceName);

		//Device Type
	//	fprintf(gpFile, "\n Device Name = %s\n", vkPhysicalDeviceproperties.deviceType);
		//
		switch (vkPhysicalDeviceproperties.deviceType)
		{

		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			fprintf(gpFile, "\n Device Type = Integrated GPU(iGPU)\n");
			break;

		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			fprintf(gpFile, "\n Device Type = Discrete GPU(dGPU)\n");
			break;

		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			fprintf(gpFile, "\n Device Type = Virtual GPU(vGPU)\n");
			break;

		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			fprintf(gpFile, "\n Device Type = CPU \n");
			break;

		case VK_PHYSICAL_DEVICE_TYPE_OTHER:
			fprintf(gpFile, "\n Device Type = Other\n");
			break;


		default:
			fprintf(gpFile, "\n Device Type = UNKNOWN\n");
			break;
		}

		//vendor id
		fprintf(gpFile, "\n Vendor ID  = 0x%04x\n", vkPhysicalDeviceproperties.vendorID);


		//device ID
		fprintf(gpFile, "\n Device ID  = 0x%04x\n", vkPhysicalDeviceproperties.deviceID);

	}

	//free global physical device array
	if (VkCore.vkPhysicalDevice_array)
	{
		free(VkCore.vkPhysicalDevice_array);
		VkCore.vkPhysicalDevice_array = NULL;
		fprintf(gpFile, "getPhysicalDevice() Succeeded to free vkPhysicalDevice_array\n");
	}




	return vkResult;


}







/////////// Device Extension Names




VkResult fillDeviceExtensionNames(void)
{
	VkCore.enabledDeviceExtensionCount = 0;
	// Variable declarations
	VkResult vkResult = VK_SUCCESS;

	//1.Find how many extensions are supported by vulkan driver of this version and keep it in local variable
	uint32_t deviceExtensionCount = 0;

	//First vulkan API
	vkResult = vkEnumerateDeviceExtensionProperties(VkCore.vkPhysicalDevice_Selected, NULL, &deviceExtensionCount, NULL);
	//NULL : Layer name supported by Vulkan,  &instanceextensioncount : count or size, NULL: Extension properties array(parameterised returm value)
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "First call to fillDeviceExtensionNames()->vkEnumerateInstanceExtensionProperties Function is Failed");
		return (vkResult);
	}
	else {
		fprintf(gpFile, "First call to fillDeviceExtensionNames()->vkEnumerateInstanceExtensionProperties Function is Succeded");
	}

	//2. Allocate and Fill VkExtensionProperties struct properties corresponding to above count  
	VkExtensionProperties* vkExtensionProperties_array = NULL;
	vkExtensionProperties_array = (VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * deviceExtensionCount);			//Allocate memory for VkExxtension
	//For the sake of bravety no malloc checking code


	vkResult = vkEnumerateDeviceExtensionProperties(VkCore.vkPhysicalDevice_Selected, NULL, &deviceExtensionCount, vkExtensionProperties_array);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "Second call to fillDeviceExtensionNames()->vkEnumerateInstanceExtensionProperties Function is Failed");
		return (vkResult);
	}
	else {
		fprintf(gpFile, "Second call to fillDeviceExtensionNames()->vkEnumerateInstanceExtensionProperties Function is Succeded");
	}


	//3. Fill and display a local string array of extension names obtained from vkExtensionProperties structure array
	char** deviceExtensionNames_array = NULL;
	deviceExtensionNames_array = (char**)malloc(sizeof(char*) * deviceExtensionCount);
	for (uint32_t i = 0; i < deviceExtensionCount; i++)
	{
		deviceExtensionNames_array[i] = (char*)malloc(sizeof(char) * strlen(vkExtensionProperties_array[i].extensionName) + 1);
		memcpy(deviceExtensionNames_array[i], vkExtensionProperties_array[i].extensionName, strlen(vkExtensionProperties_array[i].extensionName) + 1);
		fprintf(gpFile, "fillDeviceExtensionNames()->vulkanDeviceExtensionName = %s\n", deviceExtensionNames_array[i]);
	}
	fprintf(gpFile, "\n\n%s() : Device Extension Count = %u\n", __FUNCTION__, deviceExtensionCount);

	//4. As not required here onward free the vkExtension properties array
	free(vkExtensionProperties_array);
	vkExtensionProperties_array = NULL;		//bhanda swachha 


	//5.Find weather above extensions contain our required 2 extensions
	VkBool32 vulkanSwapChainExtensionFound = VK_FALSE;


	for (uint32_t i = 0; i < deviceExtensionCount; i++)
	{
		if (strcmp(deviceExtensionNames_array[i], VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
		{
			vulkanSwapChainExtensionFound = VK_TRUE;
			//VK_KHR_SWAPCHAIN_EXTENSION_NAME
			const char* enabledDeviceExtensionNames_array[1];

			VkCore.enabledDeviceExtensionNames_array[VkCore.enabledDeviceExtensionCount++] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
		}
	}

	//6 As not needed henceforth free the local strings array
	for (uint32_t i = 0; i < deviceExtensionCount; i++)
	{
		free(deviceExtensionNames_array[i]);
	}
	free(deviceExtensionNames_array);


	//7.Display weather required instance extensions supported or not
	if (vulkanSwapChainExtensionFound == VK_FALSE)
	{
		vkResult = VK_ERROR_INITIALIZATION_FAILED;			//Return hardcoded failure
		fprintf(gpFile, "fillDeviceExtensionNames()->VK_KHR_SWAPCHAIN_EXTENSION_NAME not found");
		return (vkResult);
	}
	else {
		fprintf(gpFile, "fillDeviceExtensionNames()->VK_KHR_SWAPCHAIN_EXTENSION_NAME  is found");
	}




	//8. Print only enabled extension names
	for (uint32_t i = 0; i < VkCore.enabledDeviceExtensionCount; i++)
	{
		fprintf(gpFile, "\n\nfillDeviceExtensionNames()->EnabledVulkanDeviceExtensionName = %s\n", VkCore.enabledDeviceExtensionNames_array[i]);
		return (vkResult);
	}


	return (vkResult);
}





VkResult createVulkanDevice(void)
{

	////////Function declaration
	// Function declarations
	VkResult fillDeviceExtensionNames();



	// Variable declarations
	VkResult vkResult = VK_SUCCESS;
	vkResult = fillDeviceExtensionNames();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVulkanDevice()->fillDeviceExtensionNames Function is Failed %d\n", vkResult);
		return vkResult;
	}
	else
	{
		fprintf(gpFile, "createVulkanDevice()->fillDeviceExtensionNames Function is Succeded\n");
	}

	//Newly Added
	float queuePriority = 1.0f;
	VkDeviceQueueCreateInfo vkDeviceQueueCreateInfo;
	memset((void*)&vkDeviceQueueCreateInfo, 0, sizeof(VkDeviceQueueCreateInfo));
	vkDeviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	vkDeviceQueueCreateInfo.pNext = NULL;
	vkDeviceQueueCreateInfo.flags = 0;
	vkDeviceQueueCreateInfo.queueFamilyIndex = VkCore.graphicsQueueFamilyIndex_Selected;
	vkDeviceQueueCreateInfo.queueCount = 1;
	vkDeviceQueueCreateInfo.pQueuePriorities = &queuePriority;



	//3. Initialize VkDeviceCreateInfo structure

	VkDeviceCreateInfo vkDeviceCreateInfo;
	memset((void*)&vkDeviceCreateInfo, 0, sizeof(VkDeviceCreateInfo));

	vkDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	vkDeviceCreateInfo.pNext = NULL;
	vkDeviceCreateInfo.flags = 0;
	vkDeviceCreateInfo.queueCreateInfoCount = 1;
	vkDeviceCreateInfo.pQueueCreateInfos = &vkDeviceQueueCreateInfo; //newly added
	vkDeviceCreateInfo.enabledExtensionCount = VkCore.enabledDeviceExtensionCount;
	vkDeviceCreateInfo.ppEnabledExtensionNames = VkCore.enabledDeviceExtensionNames_array;
	vkDeviceCreateInfo.enabledLayerCount = 0;
	vkDeviceCreateInfo.ppEnabledLayerNames = NULL;
	vkDeviceCreateInfo.pEnabledFeatures = NULL;

	   

	//5 Now call vkCreateDevice() vulkan api to create actually vulkan device
	vkResult = vkCreateDevice(VkCore.vkPhysicalDevice_Selected, &vkDeviceCreateInfo, NULL, &VkCore.vkDevice);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "createVulkanDevice()->vkCreateDevice Function is Failed %d\n", vkResult);
		return vkResult;
	}
	else
	{
		fprintf(gpFile, "createVulkanDevice()->vkCreateDevice Function is Succeded\n");
	}



	return vkResult;

}




void getDeviceQueue(void)
{

	//Code
	vkGetDeviceQueue(VkCore.vkDevice, VkCore.graphicsQueueFamilyIndex_Selected, 0, &VkCore.vkQueue);
	//logical channels 
	//queues are internally HW GPU cores are channels
	//its software representation means devicequeue
	//0 : queue index in the family
	if (VkCore.vkQueue == VK_NULL_HANDLE)
	{
		fprintf(gpFile, "\ngetDeviceQueue()->vkGetDeviceQueue Function returned NULL for vkQueue\n ");
		return;
	}
	else
	{
		fprintf(gpFile, "\ngetDeviceQueue()->vkGetDeviceQueue Function Succeded vkQueue\n ");

	}


	//void return value means all param are correct


}




VkResult getPhysicalDeviceSurfaceFormatAndColorSpace(void)
{

	// Variable declarations
	VkResult vkResult = VK_SUCCESS;

	VkCore.vkFormat_color = VK_FORMAT_UNDEFINED;
	VkCore.vkColorSpacrKHR = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	//Code
	//get the count of supported color surface color VkFormats
	uint32_t formatCount = 0;

	vkResult = vkGetPhysicalDeviceSurfaceFormatsKHR(VkCore.vkPhysicalDevice_Selected, VkCore.vkSurfaceKHR, &formatCount, NULL);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "getPhysicalDeviceSurfaceFormatAndColorSpace()->vkGetPhysicalDeviceSurfaceFormatsKHR Function is Failed %d\n", vkResult);
		return vkResult;
	}
	else if (formatCount == 0)
	{
		fprintf(gpFile, "vkGetPhysicalDeviceSurfaceFormatsKHR() failed due to no surface format count not present driver %d\n", vkResult);
		return vkResult;
	}
	else
	{
		fprintf(gpFile, "getPhysicalDeviceSurfaceFormatAndColorSpace()->vkGetPhysicalDeviceSurfaceFormatsKHR Function is Succeded\n");
	}


	//declare and allocate vkSurfaceFormatKHR Array
	VkSurfaceFormatKHR* vkSurfaceFormatKHR_array = (VkSurfaceFormatKHR*)malloc(formatCount * sizeof(VkSurfaceFormatKHR));


	//filling the array
	vkResult = vkGetPhysicalDeviceSurfaceFormatsKHR(VkCore.vkPhysicalDevice_Selected, VkCore.vkSurfaceKHR, &formatCount, vkSurfaceFormatKHR_array);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "getPhysicalDeviceSurfaceFormatAndColorSpace()->vkGetPhysicalDeviceSurfaceFormatsKHR second Function is Failed %d\n", vkResult);
		return vkResult;
	}
	else
	{
		fprintf(gpFile, "getPhysicalDeviceSurfaceFormatAndColorSpace()->vkGetPhysicalDeviceSurfaceFormatsKHR second Function is Succeded\n");
	}

	//Decide the surface color format first
	if (formatCount == 1 && vkSurfaceFormatKHR_array[0].format == VK_FORMAT_UNDEFINED)
	{
		VkCore.vkFormat_color = VK_FORMAT_B8G8R8A8_UNORM;
	}
	else
	{
		VkCore.vkFormat_color = vkSurfaceFormatKHR_array[0].format;
	}

	//Decide the color space
	VkCore.vkColorSpacrKHR = vkSurfaceFormatKHR_array[0].colorSpace;

	//free the array
	if (vkSurfaceFormatKHR_array)
	{
		free(vkSurfaceFormatKHR_array);
		vkSurfaceFormatKHR_array = NULL;
		fprintf(gpFile, "vkSurfaceFormatKHR_array is Free\n");
	}

	return vkResult;
}


VkResult getPhysicalDevicePresentMode(void)
{
	VkCore.vkSwapchainKHR = VK_NULL_HANDLE;

	VkCore.vkPresentModeKHR = VK_PRESENT_MODE_FIFO_KHR;

	// Variable declarations
	VkResult vkResult = VK_SUCCESS;

	//code
	uint32_t presentModeCount = 0;

	//First call
	vkResult = vkGetPhysicalDeviceSurfacePresentModesKHR(VkCore.vkPhysicalDevice_Selected, VkCore.vkSurfaceKHR, &presentModeCount, NULL);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "getPhysicalDevicePresentMode()->vkGetPhysicalDeviceSurfacePresentModesKHR first Function is Failed %d\n", vkResult);
		return vkResult;
	}
	else if (presentModeCount == 0)
	{
		fprintf(gpFile, "vkGetPhysicalDeviceSurfacePresentModesKHR() failed due to no surface format count not present driver %d\n", vkResult);
		return vkResult;
	}
	else
	{
		fprintf(gpFile, "getPhysicalDevicePresentMode()->vkGetPhysicalDeviceSurfacePresentModesKHR first Function is Succeded\n");
	}


	VkPresentModeKHR* vkPresentModeKHR_array = (VkPresentModeKHR*)malloc(presentModeCount * sizeof(VkPresentModeKHR));


	vkResult = vkGetPhysicalDeviceSurfacePresentModesKHR(VkCore.vkPhysicalDevice_Selected, VkCore.vkSurfaceKHR, &presentModeCount, vkPresentModeKHR_array);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "getPhysicalDevicePresentMode()->vkGetPhysicalDeviceSurfacePresentModesKHR second Function is Failed %d\n", vkResult);
		return vkResult;
	}
	else
	{
		fprintf(gpFile, "getPhysicalDevicePresentMode()->vkGetPhysicalDeviceSurfacePresentModesKHR second Function is Succeded\n");
	}



	//Decide the presentation Mode
	for (uint32_t i = 0; i < presentModeCount; i++)
	{
		if (vkPresentModeKHR_array[i] == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			fprintf(gpFile, "MAILBOX\n", vkPresentModeKHR_array[i]);

			VkCore.vkPresentModeKHR = VK_PRESENT_MODE_MAILBOX_KHR;
			break;
		}


	}

	if (vkPresentModeKHR_array != VK_PRESENT_MODE_MAILBOX_KHR)
	{
		fprintf(gpFile, "NOT A MAILBOX\n", vkPresentModeKHR_array);

		VkCore.vkPresentModeKHR = VK_PRESENT_MODE_FIFO_KHR;
	}

	//FREE
	if (vkPresentModeKHR_array)
	{
		free(vkPresentModeKHR_array);
		vkPresentModeKHR_array = NULL;
		fprintf(gpFile, "vkPresentModeKHR_array is Free\n");

	}

	return vkResult;


}




VkResult createSwapchain(VkBool32 vsync)
{
	// Variable declarations
	VkResult vkResult = VK_SUCCESS;

	///Function Declaration
	VkResult getPhysicalDeviceSurfaceFormatAndColorSpace(void);

	VkResult getPhysicalDevicePresentMode(void);



	///////////// Code Color format and color space

	//Step 1
	vkResult = getPhysicalDeviceSurfaceFormatAndColorSpace();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "\ncreateSwapchain()->getPhysicalDeviceSurfaceFormatAndColorSpace Function is %d\n", vkResult);
		return vkResult;
	}
	else {
		fprintf(gpFile, "\ncreateSwapchain()->getPhysicalDeviceSurfaceFormatAndColorSpace Function is Succeded");
	}

	//Step 2 GetPhysicalDeviceSurfaceCapabilities()
	//structure declare
	VkSurfaceCapabilitiesKHR vkSurfaceCapabilitiesKHR;
	memset((void*)&vkSurfaceCapabilitiesKHR, 0, sizeof(VkSurfaceCapabilitiesKHR));

	vkResult = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkCore.vkPhysicalDevice_Selected, VkCore.vkSurfaceKHR, &vkSurfaceCapabilitiesKHR);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "\ncreateSwapchain()->vkGetPhysicalDeviceSurfaceCapabilitiesKHR() Function is %d\n", vkResult);
		return vkResult;
	}
	else {
		fprintf(gpFile, "\ncreateSwapchain()->vkGetPhysicalDeviceSurfaceCapabilitiesKHR() Function is Succeded");
	}

	///Step 3 minImageCount and maxImageCount : Find out desired number of swapchain image count
	uint32_t testingNumberOfSwapchainImages = vkSurfaceCapabilitiesKHR.minImageCount + 1;
	uint32_t desiredNumberOfSwapchainImages = 0;

	if (vkSurfaceCapabilitiesKHR.maxImageCount > 0 && vkSurfaceCapabilitiesKHR.maxImageCount < testingNumberOfSwapchainImages)
	{
		desiredNumberOfSwapchainImages = vkSurfaceCapabilitiesKHR.maxImageCount;
		fprintf(gpFile, "\nmaxImageCount\n");

	}
	else
	{
		desiredNumberOfSwapchainImages = vkSurfaceCapabilitiesKHR.minImageCount;
		fprintf(gpFile, "\nminImageCount\n");

	}

	fprintf(gpFile, "\n\n\n!!!%d %d %d !!!\n", vkSurfaceCapabilitiesKHR.minImageCount, vkSurfaceCapabilitiesKHR.maxImageCount, desiredNumberOfSwapchainImages);


	///Step 4 choose size of the swapchain image

	memset((void*)&VkCore.vkExtent2D_Swapchain, 0, sizeof(VkExtent2D));
	if (vkSurfaceCapabilitiesKHR.currentExtent.width != UINT32_MAX)
	{
		VkCore.vkExtent2D_Swapchain.width = vkSurfaceCapabilitiesKHR.currentExtent.width;
		VkCore.vkExtent2D_Swapchain.height = vkSurfaceCapabilitiesKHR.currentExtent.height;
		fprintf(gpFile, "\nIn IF createSwapchain()->SwapchainImage WidthxHeight = %d x %d\n", vkSurfaceCapabilitiesKHR.currentExtent.width, vkSurfaceCapabilitiesKHR.currentExtent.height);
	}
	else
	{
		//if surfacesize is already defined thn swapchainimage size muct match with it
		VkExtent2D vkExtent2D;
		memset((void*)&vkExtent2D, 0, sizeof(VkExtent2D));

		vkExtent2D.width = (uint32_t)winWidth;
		vkExtent2D.height = (uint32_t)winHeight;

		VkCore.vkExtent2D_Swapchain.width = max(vkSurfaceCapabilitiesKHR.minImageExtent.width, min(vkSurfaceCapabilitiesKHR.maxImageExtent.width, vkExtent2D.width));
		VkCore.vkExtent2D_Swapchain.height = max(vkSurfaceCapabilitiesKHR.minImageExtent.height, min(vkSurfaceCapabilitiesKHR.maxImageExtent.height, vkExtent2D.height));
		fprintf(gpFile, "\ncreateSwapchain()->SwapchainImage Width x Height = %d x %d\n", vkSurfaceCapabilitiesKHR.currentExtent.width, vkSurfaceCapabilitiesKHR.currentExtent.height);

	}

	//Step 5 Set Swapchain image usage Flag
	VkImageUsageFlags vkImageUsageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

	//Step 6 whether to consider pre-transform/flipping or not
	VkSurfaceTransformFlagBitsKHR vkSurfaceTransformFlagBitsKHR;		//this is enum so no memset()

	if (vkSurfaceCapabilitiesKHR.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		vkSurfaceTransformFlagBitsKHR = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else
	{
		vkSurfaceTransformFlagBitsKHR = vkSurfaceCapabilitiesKHR.currentTransform;
	}

	/////////Step 7 Call Presentation Mode
	vkResult = getPhysicalDevicePresentMode();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "\ncreateSwapchain()->getPhysicalDevicePresentMode Function is %d\n", vkResult);
		return vkResult;
	}
	else {
		fprintf(gpFile, "\ncreateSwapchain()->getPhysicalDevicePresentMode Function is Succeded");
	}


	//Step 8 now fill the structure initialize VkCreateInfoSwapchain structure
	VkSwapchainCreateInfoKHR vkSwapchainCreateInfoKHR;
	memset((void*)&vkSwapchainCreateInfoKHR, 0, sizeof(VkSwapchainCreateInfoKHR));

	vkSwapchainCreateInfoKHR.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	vkSwapchainCreateInfoKHR.pNext = NULL;
	vkSwapchainCreateInfoKHR.flags = 0;
	vkSwapchainCreateInfoKHR.surface = VkCore.vkSurfaceKHR;
	vkSwapchainCreateInfoKHR.minImageCount = desiredNumberOfSwapchainImages;
	vkSwapchainCreateInfoKHR.imageFormat = VkCore.vkFormat_color;
	vkSwapchainCreateInfoKHR.imageColorSpace = VkCore.vkColorSpacrKHR;
	vkSwapchainCreateInfoKHR.imageExtent.width = VkCore.vkExtent2D_Swapchain.width;
	vkSwapchainCreateInfoKHR.imageExtent.height = VkCore.vkExtent2D_Swapchain.height;
	vkSwapchainCreateInfoKHR.imageUsage = vkImageUsageFlags;
	vkSwapchainCreateInfoKHR.preTransform = vkSurfaceTransformFlagBitsKHR;
	vkSwapchainCreateInfoKHR.imageArrayLayers = 1;
	vkSwapchainCreateInfoKHR.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	vkSwapchainCreateInfoKHR.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	vkSwapchainCreateInfoKHR.presentMode = VkCore.vkPresentModeKHR;
	vkSwapchainCreateInfoKHR.clipped = VK_TRUE;


	////STEP 9
	vkResult = vkCreateSwapchainKHR(VkCore.vkDevice, &vkSwapchainCreateInfoKHR, NULL, &VkCore.vkSwapchainKHR);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "\ncreateSwapchain()->vkCreateSwapchainKHR Function is %d\n", vkResult);
		return vkResult;
	}
	else {
		fprintf(gpFile, "\ncreateSwapchain()->vkCreateSwapchainKHR Function is Succeded");
	}


	return vkResult;



}

VkResult createImagesAndImageViews(void)
{

	// Variable declarations
	VkResult vkResult = VK_SUCCESS;

	///code
	VkCore.swapchainImageCount = UINT32_MAX;
	VkCore.swapchainImage_array = NULL;
	VkCore.swapchainImageView_array = NULL;

	//Get swapchain image count
	vkResult = vkGetSwapchainImagesKHR(VkCore.vkDevice, VkCore.vkSwapchainKHR, &VkCore.swapchainImageCount, NULL);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "\ncreateImagesAndImageViews()->vkGetSwapchainImagesKHR Function is Failed %d\n", vkResult);
		return vkResult;
	}
	else if (VkCore.swapchainImageCount == 0)
	{
		fprintf(gpFile, "\ncreateImagesAndImageViews() failed due to no count not present driver %d\n", vkResult);
		return vkResult;
	}
	else
	{
		fprintf(gpFile, "\n\ncreateImagesAndImageViews()->vkGetSwapchainImagesKHR gives swapchain (desired) image count = %d\n", VkCore.swapchainImageCount);
	}


	//Step 2 : allocate the swapchin image array
	VkCore.swapchainImage_array = (VkImage*)malloc(VkCore.swapchainImageCount * sizeof(VkImage));

	vkResult = vkGetSwapchainImagesKHR(VkCore.vkDevice, VkCore.vkSwapchainKHR, &VkCore.swapchainImageCount, VkCore.swapchainImage_array);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gpFile, "\ncreateImagesAndImageViews()->vkGetSwapchainImagesKHR Function is %d\n", vkResult);
		return vkResult;
	}
	else {
		fprintf(gpFile, "\ncreateImagesAndImageViews()->vkGetSwapchainImagesKHR Function is Succeded");
	}


	//step 3 : allocate array of swapchain image view
	VkCore.swapchainImageView_array = (VkImageView*)malloc(VkCore.swapchainImageCount * sizeof(VkImageView));

	//step5:initialize vkimageview createinfo structure

	VkImageViewCreateInfo vkImageViewCreateInfo;
	memset((void*)&vkImageViewCreateInfo, 0, sizeof(VkImageViewCreateInfo));

	vkImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	vkImageViewCreateInfo.pNext = NULL;
	vkImageViewCreateInfo.flags = 0;
	vkImageViewCreateInfo.format = VkCore.vkFormat_color;
	vkImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
	vkImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
	vkImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
	vkImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
	vkImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; //WHICH PART OF IMAGE OR WHOLE OF IMAGE IS GOING TO BE AFFECTED BY IMAGE BARRIER
	vkImageViewCreateInfo.subresourceRange.baseMipLevel = 0;		//How much mipmap level so start from 0th index level
	vkImageViewCreateInfo.subresourceRange.levelCount = 1;			//dont know level count so atleast 1 is expected
	vkImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;		//if image is composite then any layer is also image then it is called as layered rendering and it is image arrays layer
	vkImageViewCreateInfo.subresourceRange.layerCount = 1;			//minimum layer is 1
	vkImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;			//ENUM OF vk_image_view_type_2D

	//Step 6 : Now fill imageview_array using above struct
	for (uint32_t i = 0; i < VkCore.swapchainImageCount; i++)
	{
		vkImageViewCreateInfo.image = VkCore.swapchainImage_array[i];

		vkResult = vkCreateImageView(VkCore.vkDevice, &vkImageViewCreateInfo, NULL, &VkCore.swapchainImageView_array[i]);
		if (vkResult != VK_SUCCESS)
		{
			fprintf(gpFile, "\ncreateImagesAndImageViews()->vkCreateImageViews failed for iteration %d.%d\n", i, vkResult);
			return vkResult;
		}
		else
		{
			fprintf(gpFile, "\ncreateImagesAndImageViews()->vkCreateImageViews Function is Succeded");
		}

	}

	return vkResult;


}


