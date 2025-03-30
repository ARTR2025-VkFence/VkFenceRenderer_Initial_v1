

#include "VK.h"
#include"init.h"

FILE* gp_File = NULL;
gbFullScreen = FALSE;
gbActiveWindow = FALSE;
//*gpszAppName = "ARTR";

// Entry point fi=unction
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Function declarations
	VkResult Initialize(void);
	void Display(void);
	void Update(void);
	void Uninitialize(void);

	// Variable declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[255];
	BOOL bDone = FALSE;
	int iRetVal = 0;
	VkResult vkResult = VK_SUCCESS;

	

	// Code
	gp_File = fopen("WinLog.txt", "w");
	if (gp_File == NULL)
	{
		MessageBox(NULL, TEXT("\nFailed to create log file. Exiting now..."), TEXT("File I/O Error"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gp_File, "\nWinMain()->Program Started successfully.\n");
	}
	wsprintf(szAppName, "%s", "ARTR");
	// Initialize WNDCLASSEX structure
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	// Register above class
	RegisterClassEx(&wndclass);

	// Create the window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Vulkan Renderer: VkFence"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		(GetSystemMetrics(SM_CXSCREEN) - WINDOW_WIDTH) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - WINDOW_HEIGHT) / 2,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;

	// Initialize
	vkResult = Initialize();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gp_File, "\nWinMain()->Initilize Function is Failed");
		DestroyWindow(hwnd);
		hwnd = NULL;;
	}
	else {
		fprintf(gp_File, "\nWinMain()->Initilize Function is Succeded");
	}

	// Show window
	ShowWindow(hwnd, iCmdShow);

	// Bring window to foreground and set focus on it
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// Game loop
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDone = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActiveWindow == TRUE)
			{
				// Render the scene
				Display();

				// Update the scene
				Update();
			}
		}
	}

	Uninitialize();

	return ((int)msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// Function declarations
	void ToggleFullScreen(void);
	void Resize(int, int);
	void uninitialize(void);

	// Code
	switch (iMsg)
	{
	case WM_SETFOCUS:
		gbActiveWindow = TRUE;
		break;

	case WM_KILLFOCUS:
		gbActiveWindow = FALSE;
		break;

	case WM_ERASEBKGND:
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 'f':
		case 'F':
			ToggleFullScreen();
			break;

		default:
			break;
		}
		break;
	case WM_CREATE:

		memset(0, 0, 0);
		wpPrev.length = sizeof(WINDOWPLACEMENT);

		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 27:	// Escape key
			DestroyWindow(hwnd);
			break;

		default:
			break;
		}
		break;

	case WM_SIZE:
		Resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}


void ToggleFullScreen(void)
{
	// Variable declarations
	static MONITORINFO mi;

	// Code

	if (gbFullScreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);

		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi.cbSize = sizeof(MONITORINFO);

			if (GetWindowPlacement(ghwnd, &wpPrev) &&
				GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, (dwStyle & ~WS_OVERLAPPEDWINDOW));
				SetWindowPos(ghwnd,
					HWND_TOP,
					mi.rcMonitor.left,
					mi.rcMonitor.top,
					(mi.rcMonitor.right - mi.rcMonitor.left),
					(mi.rcMonitor.bottom - mi.rcMonitor.top),
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}

			ShowCursor(FALSE);
			gbFullScreen = TRUE;
		}
	}
	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, (dwStyle | WS_OVERLAPPEDWINDOW));
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
		gbFullScreen = FALSE;
	}
}


VkResult Initialize(void)
{
	
	// Calling function using the
	// function poiter

	// Variable declarations
	VkResult vkResult = VK_SUCCESS;
	//VkResult vkResult = VK_SUCCESS;

	// Code
	vkResult = fillInstanceExtensionNames();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gp_File, "\nInitialize()->fillInstanceExtensionNames Function is Failed");
		return vkResult;
	}
	else {
		fprintf(gp_File, "\nInitialize()->fillInstanceExtensionNames Function is Succeded");
	}



	//Function Pointer for Instance
	/*pfncreateInstance = (PFN_vkCreateInstance)vkGetInstanceProcAddr(NULL, "createVulkanInstance");
	if (!pfncreateInstance)
	{
		fprintf(gp_File, "\nInitialize()->createVulkanInstance Function is Failed\n");
		
	}
	else
	{
		fprintf(gp_File, "\nInitialize()->createVulkanInstance Function is Succeded\n");  
	}*/
	vkResult = createVulkanInstance();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gp_File, "\nInitialize()->createVulkanInstance Function is Failed");
		return vkResult;
	}
	else {
		fprintf(gp_File, "\nInitialize()->createVulkanInstance Function is Succeded");
	}

	fprintf(gp_File, "********************************************************************************************\n");




	////Create vulkan presentation surface
	vkResult = getSupportedSurface();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gp_File, "\nInitialize()->getSupportedSurface Function is Failed");
		return vkResult;
	}
	else {
		fprintf(gp_File, "\nInitialize()->getSupportedSurface Function is Succeded");
	}

	////enumerate and select physical device and its queue family index
	vkResult = getPhysicalDevice();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gp_File, "Initialize()->getPhysicalDevice Function is Failed %d\n", vkResult);
		return vkResult;
	}
	else {
		fprintf(gp_File, "Initialize()->getPhysicalDevice Function is Succeded\n");
	}


	//print Vulkan Info vkInfo
	vkResult = printVKInfo();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gp_File, "Initialize()->printVKInfo Function is Failed %d\n", vkResult);
		return vkResult;
	}
	else
	{
		fprintf(gp_File, "Initialize()->printVKInfo Function is Succeded\n");
	}




	//Device Extension Names
	vkResult = createVulkanDevice();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gp_File, "Initialize()->createVulkanDevice Function is Failed");
		return vkResult;
	}
	else {
		fprintf(gp_File, "Initialize()->createVulkanDevice Function is Succeded");
	}


	//get device queue
	getDeviceQueue();
	fprintf(gp_File, "\n********************************************************************************************\n");


	//Device Extension Names
	vkResult = fillDeviceExtensionNames();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gp_File, "Initialize()->fillDeviceExtensionNames Function is Failed");
		return vkResult;
	}
	else {
		fprintf(gp_File, "Initialize()->fillDeviceExtensionNames Function is Succeded");
	}




	///////////Swapchain
	vkResult = createSwapchain(VK_FALSE);
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gp_File, "\nInitialize()->createSwapchain() Function is %d\n", vkResult);
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		return vkResult;
	}
	else
	{
		fprintf(gp_File, "\nInitialize()->createSwapchain() Function is Succeded");
	}



	//create vulkan images and vulkan imageviews
	vkResult = createImagesAndImageViews();
	if (vkResult != VK_SUCCESS)
	{
		fprintf(gp_File, "\nInitialize()->createImagesAndImageViews() Function is failed %d\n", vkResult);
		vkResult = VK_ERROR_INITIALIZATION_FAILED;
		return vkResult;
	}
	else
	{
		fprintf(gp_File, "\nInitialize()->createImagesAndImageViews() Function is Succeded");
	}




	return vkResult;
}


void Resize(int width, int height)
{
	// Code
	if (height == 0)
		height = 1;
}


void Display(void)
{
	// Code
}


void Update(void)
{
	// Code
}


void Uninitialize(void)
{
	// Function declarations
	void ToggleFullScreen(void);

	// Code
	if (gbFullScreen == TRUE)
	{
		ToggleFullScreen();
	}

	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}

	//Destroy vulkan device
	if (VkCore.vkDevice)
	{
		vkDeviceWaitIdle(VkCore.vkDevice);
		fprintf(gp_File, "\n vkDeviceWaitIdle() is Done\n");




		//Free swapchain Images

		for (uint32_t i = 0; i < VkCore.swapchainImageCount; i++)
		{
			vkDestroyImageView(VkCore.vkDevice, VkCore.swapchainImageView_array[i], NULL);
			fprintf(gp_File, "\nFree swapchainImage_array images freed\n");
		}

		if (VkCore.swapchainImageView_array)
		{
			free(VkCore.swapchainImageView_array);
			VkCore.swapchainImageView_array = NULL;
		}


		if (VkCore.swapchainImage_array)
		{
			free(VkCore.swapchainImage_array);
			VkCore.swapchainImage_array = NULL;
		}

		if (VkCore.vkCommandPool)
		{
			vkDestroyCommandPool(VkCore.vkDevice, VkCore.vkCommandPool, NULL);
			VkCore.vkCommandPool = VK_NULL_HANDLE;
			fprintf(gp_File, "\n vkCommandPool is Freed\n");

		}

		//destroy 	swapchain
		if (VkCore.vkSwapchainKHR)
		{
			vkDestroySwapchainKHR(VkCore.vkDevice, VkCore.vkSwapchainKHR, NULL);
			VkCore.vkSwapchainKHR = VK_NULL_HANDLE;		//Bhanda swachha
			fprintf(gp_File, "\n vkSwapchainKHR is Freed\n");

		}


		vkDestroyDevice(VkCore.vkDevice, NULL);
		VkCore.vkDevice = VK_NULL_HANDLE;
		fprintf(gp_File, "\n vkDestroyDevice() is Done\n");

	}
	//No need to Destroy selected physical device




	if (VkCore.vkSurfaceKHR)
	{
		vkDestroySurfaceKHR(VkCore.vkInstance, VkCore.vkSurfaceKHR,NULL);
		VkCore.vkSurfaceKHR = VK_NULL_HANDLE;
		fprintf(gp_File, "\nvkDestroySurfaceKHR Succeded\n");
	}

	//Destroy vulkan device
	if (VkCore.vkDevice)
	{
		vkDeviceWaitIdle(VkCore.vkDevice);
		fprintf(gp_File, "\n vkDeviceWaitIdle() is Done\n");




		//Free swapchain Images

		for (uint32_t i = 0; i < VkCore.swapchainImageCount; i++)
		{
			vkDestroyImageView(VkCore.vkDevice, VkCore.swapchainImageView_array[i], NULL);
			fprintf(gp_File, "\nFree swapchainImage_array images freed\n");
		}

		if (VkCore.swapchainImageView_array)
		{
			free(VkCore.swapchainImageView_array);
			VkCore.swapchainImageView_array = NULL;
		}


		if (VkCore.swapchainImage_array)
		{
			free(VkCore.swapchainImage_array);
			VkCore.swapchainImage_array = NULL;
		}

		if (VkCore.vkCommandPool)
		{
			vkDestroyCommandPool(VkCore.vkDevice, VkCore.vkCommandPool, NULL);
			VkCore.vkCommandPool = VK_NULL_HANDLE;
			fprintf(gp_File, "\n vkCommandPool is Freed\n");

		}

		//destroy 	swapchain
		if (VkCore.vkSwapchainKHR)
		{
			vkDestroySwapchainKHR(VkCore.vkDevice, VkCore.vkSwapchainKHR, NULL);
			VkCore.vkSwapchainKHR = VK_NULL_HANDLE;		//Bhanda swachha
			fprintf(gp_File, "\n vkSwapchainKHR is Freed\n");
			  
		}


		vkDestroyDevice(VkCore.vkDevice, NULL);
		VkCore.vkDevice = VK_NULL_HANDLE;
		fprintf(gp_File, "\n vkDestroyDevice() is Done\n");

	}
	//No need to Destroy selected physical device




	//uninitialize/destroy vulkan instance
	if (VkCore.vkInstance)
	{
		vkDestroyInstance(VkCore.vkInstance, NULL);
		VkCore.vkInstance = VK_NULL_HANDLE;
		fprintf(gp_File, "\nvkDestroyInstance Succeded\n");
	}


	if (gp_File)
	{
		fprintf(gp_File, "\nUninitialize->Program Terminated Successfully.\n");
		fclose(gp_File);
		gp_File = NULL;
	}
}





















