#pragma once
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
#include "Rendering/Vulkan/VkTextureCache.hpp"
#include "VkCameraCache.hpp"
#include "Core/System/System.hpp"
class Vulkan;

namespace fm
{
	class Window;
	class Model;
	class VkModel;
	class VkMaterial;
	class VkTexture;
	class VkShader;
	class VkCamera;
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
		virtual void update(float dt, entt::registry& registry, EventManager&) override;
		virtual void init(EventManager& event) override;

		virtual void Start(entt::registry& registry) override;
		virtual void Stop(entt::registry& registry) override;
		~VkRenderingSystem();


	private:
		VkRenderPass				_CreateRenderPass();
		std::vector<VkCommandBuffer>_CreateCommandBuffers(VkCommandPool inPool);
		bool						_RecordCommandBuffer(entt::registry& registry, std::shared_ptr<fm::VkCamera> inCamera, VkCommandBuffer inBuffer, VkFramebuffer inFrameBuffer,
														VkRenderPass inRenderPass);
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
		//VkDescriptorSetLayout									_textureSetLayout;

		VkDescriptorPool										_descriptorPool;
		GPUSceneData		_sceneParameters;
		fm::AllocatedBuffer _sceneParameterBuffer;

		std::shared_ptr<fm::Window>		_window;

		uint32_t						_currentFrame = 0;
		GRAPHIC_API						_api = GRAPHIC_API::VULKAN;

		std::unordered_map<uint32_t, std::unique_ptr<fm::VkModel>>		_staticModels;
		std::unordered_map<uint32_t, std::unique_ptr<fm::VkMaterial>>	_materials;
		std::vector< fm::VkMaterial*>									_materialsToUpdate;
		std::unordered_map<fm::ShaderID, std::shared_ptr<fm::VkShader>> _shaders;
		std::unique_ptr<fm::VkTextureCache>								_textureCache;
		std::unique_ptr<fm::VkCameraCache>								_cameraCache;


		fm::VkMaterial*				_currentMaterial;
		uint64_t					_frame = 0;
		UploadContext				_uploadContext;
	};
}