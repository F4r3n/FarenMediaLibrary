#pragma once
#include <System.h>
#include <memory>
#include <vector>
#include "vulkan/vulkan.h"
#include "Window.h"
#include "Rendering/Vulkan/VkPipelineBuilder.hpp"
#include "Rendering/Vulkan/VkVertexBuffer.hpp"
#include "Rendering/GraphicsAPI.h"
#include "Core/Math/Matrix.h"
#include <unordered_map>
#include <array>
#include "Rendering/GPUData.hpp"
#include "Rendering/ShaderKind.hpp"
#include "Rendering/SubShader.hpp"
class Vulkan;

namespace fm
{
	class Window;
	class Model;
	class VkModel;
	class VkMaterial;
	class VkTexture;
	class VkShader;
}

namespace fms
{
	struct UploadContext {
		VkFence			_uploadFence;
		VkCommandPool	_commandPool;
		VkCommandBuffer _commandBuffer;
	};


	struct FrameData
	{
		VkDescriptorSet		globalDescriptorSet;
		VkDescriptorSet		objectDescriptor;
		fm::AllocatedBuffer globalBuffer;
		fm::AllocatedBuffer objectBuffer;
	};



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
		bool						_RecordCommandBuffer(VkCommandBuffer inBuffer, VkFramebuffer inFrameBuffer,
														VkRenderPass inRenderPass, EntityManager& inManager);
		bool						_SetupSyncObjects();
		bool						_SetupGlobalUniforms();
		bool						_SetupDescriptors();
		void						_InitStandardShapes();
		bool						_SetupUploadContext();
		void						_ImmediateSubmit(std::function<void(VkCommandBuffer)>&& function);
		void						_UploadMesh(fm::VkModel* inModel);
		std::shared_ptr<fm::VkShader> _FindOrCreateShader(const fm::SubShader& inSubShader);

	private:
		inline const static int MAX_FRAMES_IN_FLIGHT = 2;

		std::unique_ptr<Vulkan>		_vulkan;
		VkRenderPass				_renderPass;


		std::vector<VkCommandBuffer>	_commandBuffers;
		std::vector<VkSemaphore>		_imageAvailableSemaphores;
		std::vector<VkSemaphore>		_renderFinishedSemaphores;
		std::vector<VkFence>			_inFlightFences;

		std::array<FrameData, MAX_FRAMES_IN_FLIGHT>				_framesData;
		VkDescriptorSetLayout									_globalSetLayout;
		VkDescriptorSetLayout									_objectSetLayout;
		VkDescriptorSetLayout									_textureSetLayout;

		VkDescriptorPool										_descriptorPool;
		GPUSceneData		_sceneParameters;
		fm::AllocatedBuffer _sceneParameterBuffer;

		std::shared_ptr<fm::Window>		_window;

		uint32_t						_currentFrame = 0;
		GRAPHIC_API						_api = GRAPHIC_API::VULKAN;

		std::map<uint32_t, std::unique_ptr<fm::VkModel>>		_staticModels;
		std::map<uint32_t, std::unique_ptr<fm::VkMaterial>>		_materials;
		std::vector< fm::VkMaterial*>							_materialsToUpdate;
		std::unordered_map<uint32_t, std::unique_ptr<fm::VkTexture>> _textures;
		std::unordered_map<fm::ShaderID, std::shared_ptr<fm::VkShader>> _shaders;


		fm::VkMaterial*				_currentMaterial;
		uint64_t					_frame = 0;
		UploadContext				_uploadContext;
	};
}