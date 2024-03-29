#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <functional>
#include "vk_mem_alloc.h"
#include <optional>
struct SDL_Window;

namespace fm
{
	struct AllocatedBuffer
	{
		VkBuffer		_buffer = nullptr;
		VmaAllocation	_allocation = nullptr;
	};

	struct AllocatedImage {
		VkImage _image = nullptr;
		VmaAllocation _allocation = nullptr;
	};
}

namespace vk_init
{
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	VkCommandBufferBeginInfo			CreateCommandBufferBeginInfo(VkCommandBufferUsageFlags flags);
	VkSubmitInfo						CreateSubmitInfo(const std::vector<VkSemaphore>& inWaitSemaphore,
											const std::vector<VkPipelineStageFlags>& inWaitPipelineStages,
											const std::vector<VkSemaphore>& inSignalSemaphores,
											VkCommandBuffer* cmd);
	VkSubmitInfo						CreateSubmitInfo(VkCommandBuffer* cmd);
	VkCommandPoolCreateInfo				CreateCommandPoolCreateInfo(QueueFamilyIndices inQueueFamily);

	VkImageCreateInfo 					CreateImageInfo(VkFormat format, VkImageUsageFlags usageFlags, VkExtent3D extent);
	VkImageViewCreateInfo				CreateImageViewInfo(VkFormat format, VkImage image, VkImageAspectFlags aspectFlags);
	VkCommandBufferAllocateInfo			CreateCommandBufferAllocateInfo(VkCommandPool pool);
	VkVertexInputAttributeDescription	CreateVertexInputAttributeDescription(uint32_t inBinding, uint32_t inLocation, VkFormat inFormat, uint32_t inOffset);
	VkDescriptorSetLayoutBinding		CreateDescriptorSetLayoutBinding(VkDescriptorType type, VkShaderStageFlags stageFlags, uint32_t binding);
	VkWriteDescriptorSet				CreateWriteDescriptorSet(VkDescriptorType type, VkDescriptorSet dstSet, VkDescriptorBufferInfo* bufferInfo, uint32_t binding);
	VkWriteDescriptorSet				CreateWriteDescriptorSet(VkDescriptorType type, VkDescriptorSet dstSet, VkDescriptorImageInfo* imageInfo, uint32_t binding);

	VkSamplerCreateInfo					CreateSamplerInfo(VkFilter filters, VkSamplerAddressMode samplerAddressMode);
	VkWriteDescriptorSet				CreateImageDescriptorSet(VkDescriptorType type, VkDescriptorSet dstSet, VkDescriptorImageInfo* imageInfo, uint32_t binding);

}


class Vulkan
{


	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct AllocatedImage
	{
		VkImage _image;
		VmaAllocation _allocation;
	};


public:



	bool Init(SDL_Window* inWindow);
	bool DeInit();

	VkQueue						GetGraphicsQueue() const { return _graphicsQueue; }
	VkDevice					GetDevice() const { return _device; }
	VkCommandPool				GetCommandPool() const { return _commandPool; }
	VmaAllocator				GetAllocator() const { return _allocator; }
	VkFormat					GetDepthFormat() const { return _depthFormat; }
	vk_init::QueueFamilyIndices	GetQueueFamilyIndices() const { return _queueFamilyIndices; }

	//Swap chain related
	VkFramebuffer				GetSwapChainFrameBuffer(uint32_t index) const { return _swapChainFramebuffers[index]; }
	VkExtent2D					GetSwapChainExtent() const { return _swapChainExtent; }
	VkFormat					GetSwapChainFormat() const { return _swapChainImageFormat; }
	std::vector<VkImageView>	GetSwapChainImageViews() const { return _swapChainImageViews; }
	bool						AcquireImage(VkSemaphore inSemaphore, uint32_t &imageIndex, SDL_Window* inWindow, VkRenderPass inRenderPass);
	void						SubmitPresentQueue(VkSemaphore* inSemaphores, uint32_t inImageIndex, SDL_Window* inWindow, VkRenderPass inRenderPass);
	bool						SetupSwapChainFramebuffer(VkRenderPass inRenderPass);
	bool						SetupDepthImage(VkExtent2D inExtent);

	fm::AllocatedBuffer			CreateBuffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);
	fm::AllocatedBuffer			CreateBuffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlagBits inFlags);

	bool						DestroyBuffer(fm::AllocatedBuffer& inBuffer) const;
	bool						DestroyImage(fm::AllocatedImage& inBuffer) const;

	void*						MapBuffer(fm::AllocatedBuffer& inBuffer, void* inData, size_t inDataSize, size_t inOffset) const;
	void						MapBuffer(fm::AllocatedBuffer& inBuffer, std::function<void(void**)> && inFunction) const;

	size_t							PadUniformBufferSize(size_t originalSize) const;
	VkDescriptorSetLayout			CreateDescriporSetLayout(const std::vector< VkDescriptorSetLayoutBinding>& inBindings) const;
	bool							AllocateDescriptorSet(VkDescriptorPool inPool, VkDescriptorSet* inSet, VkDescriptorSetLayout* inLayout) const;

private:
	void				_CreateSurface(SDL_Window* inWindow);
	bool				_SetupDebugMessenger();
	bool				_InitInstance(const std::vector<const char*>& inValidationLayerSupport);
	bool				_CheckValidationLayerSupport(const std::vector<const char*> inValidationLayerSupport);
	bool				_SetupPhysicalDevice(const std::vector<const char*>& deviceExtensions);
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
	bool				_SetupSwapChainImageViews(VkDevice inDevice);


	bool _SetUpCommandPool(VkDevice inDevice);
	void _CleanUpSwapChain();

	//Setup allocator
	bool _SetupAllocator(VkPhysicalDevice physicalDevice, VkDevice inDevice, VkInstance instance);
private:
	VkDebugUtilsMessengerEXT _debugMessenger = nullptr;

	bool			_enableValidationLayers = true;
	VkInstance		_instance = nullptr;
	VkDevice		_device = nullptr;
	VkSurfaceKHR	_surface = nullptr;
	VkPhysicalDevice _physicalDevice = nullptr;

	VkQueue			_graphicsQueue = nullptr;
	VkQueue			_presentQueue = nullptr;

	//Swap chain
	VkSwapchainKHR				_swapChain = nullptr;
	std::vector<VkImage>		_swapChainImages;
	VkFormat					_swapChainImageFormat;
	VkExtent2D					_swapChainExtent;
	std::vector<VkImageView>	_swapChainImageViews;
	std::vector<VkFramebuffer>	_swapChainFramebuffers;

	//Depth
	VkImageView		_depthImageView = nullptr;
	AllocatedImage	_depthImage;
	VkFormat		_depthFormat;


	VkCommandPool	_commandPool = nullptr;

	VmaAllocator	_allocator = nullptr; //vma lib allocator
	VkPhysicalDeviceProperties			_gpuProperties;
	vk_init::QueueFamilyIndices			_queueFamilyIndices;
};