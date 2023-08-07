#include <vulkan/vulkan.h>
#include <vector>

struct SDL_Window;


class Vulkan
{
public:
	bool Init(SDL_Window* inWindow);
	bool DeInit();

private:
	void _CreateSurface(SDL_Window* inWindow);
	bool _SetupDebugMessenger();
	bool _InitInstance(const std::vector<const char*> inValidationLayerSupport);
	bool _CheckValidationLayerSupport(const std::vector<const char*> inValidationLayerSupport);
	VkPhysicalDevice _PickPhysicalDevice() const;
	bool		_IsDeviceSuitable(VkPhysicalDevice device) const;
	VkDevice	_CreateLogicalDevice(VkPhysicalDevice device, const std::vector<const char*> inValidationLayerSupport) const;
	
private:
	VkDebugUtilsMessengerEXT _debugMessenger;

	bool		_enableValidationLayers = true;
	VkInstance  _instance = nullptr;
	VkDevice	_device = nullptr;

	VkSurfaceKHR _surface = nullptr;
};