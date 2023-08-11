#pragma once
#include <System.h>
#include <memory>
#include <vector>
#include "vulkan/vulkan.h"
#include "Window.h"
#include "Rendering/Vulkan/VkPipelineBuilder.h"
#include "Rendering/Vulkan/VkVertexBuffer.hpp"
class Vulkan;

namespace fm
{
	class Window;
}

namespace fms
{
	class VkRenderingSystem : public System<VkRenderingSystem>
	{
	public:
		VkRenderingSystem(std::shared_ptr<fm::Window> inWindow);
		virtual void pre_update(EntityManager& manager);
		virtual void update(float dt, EntityManager& manager, EventManager& event);
		virtual void init(EntityManager& manager, EventManager& event);
		virtual void over();
		virtual void Start();
		virtual void Stop();
		~VkRenderingSystem();
	private:
		VkRenderPass				_CreateRenderPass();
		std::vector<VkCommandBuffer>_CreateCommandBuffers(VkCommandPool inPool);
		bool						_RecordCommandBuffer(VkCommandBuffer inBuffer, uint32_t imageIndex,
														VkRenderPass inRenderPass, VkPipeline inPipeline);
		bool						_SetupSyncObjects();
	private:
		std::unique_ptr<Vulkan>		_vulkan;
		fm::VkPipelineBuilder		_pipeline;
		VkRenderPass				_renderPass;


		std::vector<VkCommandBuffer>	_commandBuffers;
		std::vector<VkSemaphore>		_imageAvailableSemaphores;
		std::vector<VkSemaphore>		_renderFinishedSemaphores;
		std::vector<VkFence>			_inFlightFences;

		std::shared_ptr<fm::Window>		_window;

		fm::VkVertexBuffer				_triangle;

		uint32_t currentFrame = 0;
	};
}