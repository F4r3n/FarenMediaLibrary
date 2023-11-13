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
	struct VkTextureCache;
	class VkMaterial
	{
	public:

		struct Frame
		{
			VkDescriptorSet		descriptorSet;
			unsigned char		descriptorSetReady;
			size_t				stamp;
		};

		struct SetInfo
		{
			VkDescriptorSetLayout				layout = nullptr;
			uint32_t							setPoint = 0;
			std::vector<Frame>					frames;
			Frame& GetFrame(unsigned int inFrameNumber)
			{
				if (inFrameNumber >= frames.size())
					return frames.front();
				return frames[inFrameNumber];
			}
		};

		struct MaterialBuffer
		{
			fm::AllocatedBuffer			buffer;
			uint32_t					maxBufferSize = 0;

			uint32_t					bindingPoint;
		};

		struct MaterialBufferTexture
		{
			std::shared_ptr<fm::VkTexture>		vkTexture;
			std::weak_ptr<fm::Texture>			texture;

			uint32_t							bindingPoint;
		};

		struct VkMaterialCreateInfo
		{
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
		void Update(VkDescriptorPool inPool, VkTextureCache& inCache);
		VkPipelineLayout GetPipelineLayout() const;
		bool BindSet(VkCommandBuffer inBuffer, uint32_t inFrameNumber);

		bool IsReady() const;
	private:
		std::shared_ptr<Material>			_material = nullptr;
		std::unique_ptr<VkPipelineBuilder>	_pipeline = nullptr;
		Vulkan*								_vulkan = nullptr;

		MaterialBuffer						_materialBuffer;
		SetInfo								_materialBufferInfo;

		std::vector<MaterialBufferTexture>	_materialBufferTextures;
		SetInfo								_materialBufferTextureInfo;
	};
}