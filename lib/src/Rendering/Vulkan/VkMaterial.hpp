#pragma once
#include <memory>
#include "Vulkan.hpp"
#include <vector>
#include "Rendering/SubShader.hpp"
namespace fm
{
	class Material;
	class VkPipelineBuilder;
	class VkTexture;
	class VkShader;
	class VkMaterial
	{
	public:

		struct MaterialBuffer
		{
			VkDescriptorSetLayout				bufferLayout = nullptr;
			std::vector<VkDescriptorSet>		descriptorSets;
			fm::AllocatedBuffer					buffer;

			uint32_t							bindingPoint = 0;
			uint32_t							setPoint = 0;
			uint32_t							maxBufferSize = 0;

			bool								descriptorSetReady = false;
			std::vector<size_t>					stamps;
		};

		struct VkMaterialCreateInfo {
			std::shared_ptr<fm::Material>		material;
			Vulkan*								vulkan;
			VkRenderPass						renderPass;
			VkExtent2D							extent;
			std::vector<VkDescriptorSetLayout>	descriptorLayout;
			std::vector<VkTexture*>				textures;
			VkDescriptorSetLayout				textureLayout;
			size_t								maxFramesInFlight = 1;
			std::shared_ptr<fm::VkShader>		shader;
		};

		VkMaterial(const VkMaterialCreateInfo& inInfo);

		void Destroy();
		void BindPipeline(VkCommandBuffer cmd, VkPipelineBindPoint inType);
		void Update(VkDescriptorPool inPool);
		VkPipelineLayout GetPipelineLayout() const;
		bool BindSet(VkCommandBuffer inBuffer, uint32_t inFrameNumber);

		bool isReady = false;
	private:
		std::shared_ptr<Material>			_material = nullptr;
		std::unique_ptr<VkPipelineBuilder>	_pipeline = nullptr;
		std::vector<VkTexture*>				_textures;
		std::vector<VkDescriptorSet>		_textureDescriptorSets;
		VkDescriptorSetLayout				_textureLayout;
		Vulkan*								_vulkan = nullptr;

		MaterialBuffer						_materialBuffer;
	};
}