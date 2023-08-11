#include <vulkan/vulkan.h>
#include <vector>
#include "vk_mem_alloc.h"
struct SDL_Window;


class Vulkan
{
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

public:
	bool Init(SDL_Window* inWindow);
	bool DeInit();

	VkQueue						GetGraphicsQueue() const { return _graphicsQueue; }
	VkDevice					GetDevice() const { return _device; }
	VkCommandPool				GetCommandPool() const { return _commandPool; }
	VmaAllocator				GetAllocator() const { return _allocator; }

	//Swap chain related
	VkFramebuffer				GetSwapChainFrameBuffer(uint32_t index) const { return _swapChainFramebuffers[index]; }
	VkExtent2D					GetSwapChainExtent() const { return _swapChainExtent; }
	VkFormat					GetSwapChainFormat() const { return _swapChainImageFormat; }
	std::vector<VkImageView>	GetSwapChainImageViews() const { return _swapChainImageViews; }
	bool						AcquireImage(VkSemaphore inSemaphore, uint32_t &imageIndex, SDL_Window* inWindow, VkRenderPass inRenderPass);
	void						SubmitPresentQueue(VkSemaphore* inSemaphores, uint32_t inImageIndex, SDL_Window* inWindow, VkRenderPass inRenderPass);
	bool						SetupSwapChainFramebuffer(VkRenderPass inRenderPass);


private:
	void				_CreateSurface(SDL_Window* inWindow);
	bool				_SetupDebugMessenger();
	bool				_InitInstance(const std::vector<const char*>& inValidationLayerSupport);
	bool				_CheckValidationLayerSupport(const std::vector<const char*> inValidationLayerSupport);
	VkPhysicalDevice	_PickPhysicalDevice(const std::vector<const char*>& deviceExtensions) const;
	bool				_IsDeviceSuitable(VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions) const;
	bool				_CheckDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions) const;
	bool				_SetupLogicalDevice(VkPhysicalDevice device, const std::vector<const char*>& inValidationLayerSupport, const std::vector<const char*>& deviceExtensions);
	SwapChainSupportDetails _QuerySwapChainSupport(VkPhysicalDevice device) const;

	//Setup swap chain
	bool				_SetupSwapChain(SDL_Window* inWindow, VkPhysicalDevice device);
	VkPresentModeKHR	_ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const;
	VkSurfaceFormatKHR	_ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
	VkExtent2D			_ChooseSwapExtent(SDL_Window* inWindow, const VkSurfaceCapabilitiesKHR& capabilities) const;
	bool				_RecreateSwapChain(SDL_Window* inWindow, VkRenderPass inRenderPass);

	//Setup imageview
	bool _SetupImageViews(VkDevice inDevice);
	bool _SetUpCommandPool(VkPhysicalDevice physicalDevice, VkDevice inDevice);
	void _CleanUpSwapChain();

	//Setup allocator
	bool _SetupAllocator(VkPhysicalDevice physicalDevice, VkDevice inDevice, VkInstance instance);
private:
	VkDebugUtilsMessengerEXT _debugMessenger;

	bool			_enableValidationLayers = true;
	VkInstance		_instance = nullptr;
	VkDevice		_device = nullptr;
	VkSurfaceKHR	_surface = nullptr;
	VkPhysicalDevice _physicalDevice = nullptr;

	VkQueue			_graphicsQueue;
	VkQueue			_presentQueue;

	VkSwapchainKHR			_swapChain;
	std::vector<VkImage>	_swapChainImages;
	VkFormat				_swapChainImageFormat;
	VkExtent2D				_swapChainExtent;
	std::vector<VkImageView> _swapChainImageViews;
	std::vector<VkFramebuffer>	_swapChainFramebuffers;


	VkCommandPool	_commandPool;

	VmaAllocator _allocator; //vma lib allocator
};