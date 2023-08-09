#pragma once
#include <System.h>
#include <memory>
#include "vulkan/vulkan.h"
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
		VkRenderingSystem(fm::Window& inWindow);
		virtual void pre_update(EntityManager& manager);
		virtual void update(float dt, EntityManager& manager, EventManager& event);
		virtual void init(EntityManager& manager, EventManager& event);
		virtual void over();
		virtual void Start();
		virtual void Stop();
		~VkRenderingSystem();
	private:
		VkPipelineLayout	_CreatePipelineLayout();
		VkPipeline			_CreatePipeline(VkPipelineLayout inLayout, VkRenderPass inRenderPass);

		VkRenderPass		_CreateRenderPass();

	private:
		std::unique_ptr<Vulkan>	_vulkan;
		VkPipelineLayout		_mainPipelineLayout;
		VkPipeline				_mainPipeline;

		VkRenderPass			_renderPass;
	};
}