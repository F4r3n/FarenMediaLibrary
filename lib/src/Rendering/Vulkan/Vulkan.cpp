#include "Vulkan.h"
#undef max
#include <limits>
#include <SDL3/SDL_vulkan.h>
#include <SDL3/SDL.h>
#include <optional>
#include <iostream>
#include <cstring>
#include <set>
#include <algorithm>
#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}


bool Vulkan::Init(SDL_Window* inWindow)
{
	const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	_InitInstance(validationLayers);
	if (_instance == nullptr)
		return false;

	_SetupDebugMessenger();

	_CreateSurface(inWindow);

	VkPhysicalDevice physicalDevice = _PickPhysicalDevice(deviceExtensions);
	if (physicalDevice == nullptr)
		return false;
	_physicalDevice = physicalDevice;

	if (!_SetupLogicalDevice(physicalDevice, validationLayers, deviceExtensions))
		return false;

	if (!_SetupSwapChain(inWindow, physicalDevice))
		return false;

	if (!_SetupImageViews(_device))
		return false;

	if (!_SetUpCommandPool(physicalDevice, _device))
		return false;

	if (!_SetupAllocator(physicalDevice, _device, _instance))
		return false;

	return true;
}

bool Vulkan::_SetupAllocator(VkPhysicalDevice physicalDevice, VkDevice inDevice, VkInstance instance)
{
	VmaAllocatorCreateInfo allocatorInfo = {};
	allocatorInfo.physicalDevice = physicalDevice;
	allocatorInfo.device = inDevice;
	allocatorInfo.instance = instance;
	VkResult result = vmaCreateAllocator(&allocatorInfo, &_allocator);
	return result == VK_SUCCESS;
}


bool Vulkan::AcquireImage(VkSemaphore inSemaphore, uint32_t &imageIndex, SDL_Window* inWindow, VkRenderPass inRenderPass)
{
	VkResult result = vkAcquireNextImageKHR(_device, _swapChain, UINT64_MAX, inSemaphore, VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		_RecreateSwapChain(inWindow, inRenderPass);
		return false;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		return false;
	}

	return true;
}

void Vulkan::SubmitPresentQueue(VkSemaphore* inSemaphore, uint32_t inImageIndex, SDL_Window* inWindow, VkRenderPass inRenderPass)
{
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = inSemaphore;

	VkSwapchainKHR swapChains[] = { _swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &inImageIndex;
	presentInfo.pResults = nullptr; // Optional

	VkResult result = vkQueuePresentKHR(_presentQueue, &presentInfo);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		_RecreateSwapChain(inWindow, inRenderPass);
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}
}



bool Vulkan::DeInit()
{
	vkDestroyCommandPool(_device, _commandPool, nullptr);
	_CleanUpSwapChain();

	vmaDestroyAllocator(_allocator);

	vkDestroyDevice(_device, nullptr);
	if (_enableValidationLayers) {
		DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
	}
	vkDestroySurfaceKHR(_instance, _surface, nullptr);

	vkDestroyInstance(_instance, nullptr);

	return true;
}



void Vulkan::_CreateSurface(SDL_Window* inWindow)
{
	SDL_Vulkan_CreateSurface(inWindow, _instance, &_surface);
}

bool Vulkan::_SetupDebugMessenger() {
	if (!_enableValidationLayers) return true;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(_instance, &createInfo, nullptr, &_debugMessenger) != VK_SUCCESS) {
		return false;
	}
	return true;
}


bool Vulkan::_InitInstance(const std::vector<const char*>& inValidationLayerSupport)
{
	if (_enableValidationLayers && !_CheckValidationLayerSupport(inValidationLayerSupport)) {
		return false;
	}

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;

	unsigned int extensionCount = 0;
	SDL_Vulkan_GetInstanceExtensions(&extensionCount, nullptr);
	std::vector<const char*> extensionNames(extensionCount);
	SDL_Vulkan_GetInstanceExtensions(&extensionCount, extensionNames.data());

	//extensionNames.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
	extensionNames.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	extensionNames.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = extensionNames.size();
	createInfo.ppEnabledExtensionNames = extensionNames.data();
	//createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (_enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(inValidationLayerSupport.size());
		createInfo.ppEnabledLayerNames = inValidationLayerSupport.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else {
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS) {
		return false;
	}
	return true;
}


struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR inSurface) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());



	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, inSurface, &presentSupport);

		if (presentSupport)
		{
			indices.presentFamily = i;
		}

		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}
		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}

bool Vulkan::_SetUpCommandPool(VkPhysicalDevice physicalDevice, VkDevice inDevice)
{
	QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(physicalDevice, _surface);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(inDevice, &poolInfo, nullptr, &_commandPool) != VK_SUCCESS) {
		return false;
	}
	return true;
}


bool Vulkan::_CheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions) const
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

Vulkan::SwapChainSupportDetails Vulkan::_QuerySwapChainSupport(VkPhysicalDevice device) const
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}


bool Vulkan::_IsDeviceSuitable(VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions) const
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	//TODO: set score

	QueueFamilyIndices indices = FindQueueFamilies(device, _surface);
	bool extensionsSupported = _CheckDeviceExtensionSupport(device, deviceExtensions);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = _QuerySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate;
}


VkPhysicalDevice Vulkan::_PickPhysicalDevice(const std::vector<const char*>& deviceExtensions) const
{
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		return VK_NULL_HANDLE;
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		if (_IsDeviceSuitable(device, deviceExtensions)) {
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE) {
		return VK_NULL_HANDLE;
	}

	return physicalDevice;
}

bool Vulkan::_SetupLogicalDevice(VkPhysicalDevice device, const std::vector<const char*>& inValidationLayerSupport,
	const std::vector<const char*>& deviceExtensions)
{
	QueueFamilyIndices indices = FindQueueFamilies(device, _surface);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = queueCreateInfos.size();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (_enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(inValidationLayerSupport.size());
		createInfo.ppEnabledLayerNames = inValidationLayerSupport.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(device, &createInfo, nullptr, &_device) != VK_SUCCESS) {
		return false;
	}


	vkGetDeviceQueue(_device, indices.graphicsFamily.value(), 0, &_graphicsQueue);
	vkGetDeviceQueue(_device, indices.presentFamily.value(), 0, &_presentQueue);

	return _device != nullptr;
}


bool Vulkan::_CheckValidationLayerSupport(const std::vector<const char*> inValidationLayerSupport) {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : inValidationLayerSupport) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

VkPresentModeKHR Vulkan::_ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkSurfaceFormatKHR Vulkan::_ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkExtent2D Vulkan::_ChooseSwapExtent(SDL_Window* inWindow, const VkSurfaceCapabilitiesKHR& capabilities) const
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		SDL_GetWindowSizeInPixels(inWindow, &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}


bool Vulkan::_SetupSwapChain(SDL_Window* inWindow, VkPhysicalDevice device)
{
	SwapChainSupportDetails swapChainSupport = _QuerySwapChainSupport(device);

	VkSurfaceFormatKHR surfaceFormat = _ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = _ChooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = _ChooseSwapExtent(inWindow, swapChainSupport.capabilities);
	_swapChainExtent = extent;
	_swapChainImageFormat = surfaceFormat.format;

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = _surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; //VK_IMAGE_USAGE_TRANSFER_DST_BIT  for post processing

	QueueFamilyIndices indices = FindQueueFamilies(device, _surface);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE; //for resize

	if (vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapChain) != VK_SUCCESS) {
		return false;
	}

	vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, nullptr);
	_swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, _swapChainImages.data());

	return true;
}


bool Vulkan::_SetupImageViews(VkDevice inDevice)
{
	_swapChainImageViews.resize(_swapChainImages.size());

	for (size_t i = 0; i < _swapChainImages.size(); i++) {
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = _swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = _swapChainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(inDevice, &createInfo, nullptr, &_swapChainImageViews[i]) != VK_SUCCESS) {
			return false;
		}
	}
	return true;
}

bool Vulkan::SetupSwapChainFramebuffer(VkRenderPass inRenderPass)
{
	auto swapChainImageViews = _swapChainImageViews;
	_swapChainFramebuffers.resize(swapChainImageViews.size());

	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		VkImageView attachments[] = {
			swapChainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = inRenderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = _swapChainExtent.width;
		framebufferInfo.height = _swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(_device, &framebufferInfo, nullptr, &_swapChainFramebuffers[i]) != VK_SUCCESS) {
			return false;
		}
	}
	return true;
	
}


void Vulkan::_CleanUpSwapChain()
{
	for (auto imageView : _swapChainImageViews)
	{
		vkDestroyImageView(_device, imageView, nullptr);
	}

	for (auto framebuffer : _swapChainFramebuffers)
	{
		vkDestroyFramebuffer(_device, framebuffer, nullptr);
	}

	vkDestroySwapchainKHR(_device, _swapChain, nullptr);
}


bool Vulkan::_RecreateSwapChain(SDL_Window* inWindow, VkRenderPass inRenderPass)
{
	vkDeviceWaitIdle(_device);

	_CleanUpSwapChain();

	_SetupSwapChain(inWindow, _physicalDevice);
	_SetupImageViews(_device);
	SetupSwapChainFramebuffer(inRenderPass);


	return true;
}