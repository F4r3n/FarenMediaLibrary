#pragma once
#include <System.h>
#include <memory>
#include <vector>
#include "vulkan/vulkan.h"
#include "Window.h"
#include "Rendering/Vulkan/VkPipelineBuilder.h"
#include "Rendering/Vulkan/VkVertexBuffer.hpp"
#include "Rendering/GraphicsAPI.h"
#include "Core/Math/Matrix.h"
#include <unordered_map>
#include <array>
class Vulkan;

namespace fm
{
	class Window;
	class Model;
	class VkModel;
	class VkMaterial;
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
														VkRenderPass inRenderPass, EntityManager& inManager);
		bool						_SetupSyncObjects();
		bool						_SetupGlobalUniforms();
		bool						_SetupDescriptors();
		void						_InitStandardShapes();

	private:
		inline const static int MAX_FRAMES_IN_FLIGHT = 2;

		std::unique_ptr<Vulkan>		_vulkan;
		VkRenderPass				_renderPass;


		std::vector<VkCommandBuffer>	_commandBuffers;
		std::vector<VkSemaphore>		_imageAvailableSemaphores;
		std::vector<VkSemaphore>		_renderFinishedSemaphores;
		std::vector<VkFence>			_inFlightFences;

		
		std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT>	_globalDescriptorSets;
		std::array<fm::AllocatedBuffer, MAX_FRAMES_IN_FLIGHT> _globalBuffers;
		VkDescriptorSetLayout				_globalSetLayout;
		VkDescriptorPool					_descriptorPool;

		std::shared_ptr<fm::Window>		_window;

		uint32_t						_currentFrame = 0;
		GRAPHIC_API						_api = GRAPHIC_API::VULKAN;

		std::map<uint32_t, std::unique_ptr<fm::VkModel>> _staticModels;
		std::map < uint32_t, std::unique_ptr<fm::VkMaterial>>	_materials;

		std::shared_ptr<fm::Model>	_modelToDrawTest;
		fm::VkMaterial*				_currentMaterial;
	};
}