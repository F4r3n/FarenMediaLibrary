#pragma once
#include <memory>
#include "Vulkan.h"
#include <vector>
namespace fm
{
	class Material;
	class VkPipelineBuilder;
	class VkTexture;

	class VkMaterial
	{
	public:
		struct VkMaterialCreateInfo {
			std::shared_ptr<fm::Material> material;
			Vulkan* vulkan;
			VkRenderPass renderPass;
			VkExtent2D extent;
			std::vector<VkDescriptorSetLayout> descriptorLayout;
			std::vector<VkTexture*>				textures;
			VkDescriptorSetLayout				textureLayout;
		};

		VkMaterial(const VkMaterialCreateInfo& inInfo);

		void Destroy();
		void BindPipeline(VkCommandBuffer cmd, VkPipelineBindPoint inType);
		void Update(VkDescriptorPool inPool);
		VkPipelineLayout GetPipelineLayout() const;
		bool BindSet(VkCommandBuffer inBuffer);

		bool isReady = false;
	private:
		std::shared_ptr<Material>			_material = nullptr;
		std::unique_ptr<VkPipelineBuilder>	_pipeline = nullptr;
		std::vector<VkTexture*>				_textures;
		VkDescriptorSet						_textureDescriptorSet;
		VkDescriptorSetLayout				_textureLayout;
		Vulkan*								_vulkan = nullptr;
	};
}