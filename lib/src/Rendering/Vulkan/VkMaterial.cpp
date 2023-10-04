#include "VkMaterial.hpp"
#include "VkPipelineBuilder.hpp"
#include "Rendering/material.hpp"
#include "Rendering/Vulkan/VkTexture.hpp"
using namespace fm;

VkMaterial::VkMaterial(const VkMaterialCreateInfo& inInfo)
{
	_textures = inInfo.textures;
	_material = inInfo.material;
	_textureLayout = inInfo.textureLayout;
	_vulkan = inInfo.vulkan;
	_textureDescriptorSets.resize(inInfo.maxFramesInFlight);
	auto layouts = inInfo.descriptorLayout;

	if (!_textures.empty()) //The texture cannot be set during rendering, should wait for next frame
	{
		layouts.emplace_back(_textureLayout);
		isReady = false;
	}

	_pipeline = std::make_unique<fm::VkPipelineBuilder>(_vulkan->GetDevice(), inInfo.renderPass, inInfo.extent, layouts,
		_material->GetShader().get());
}


void VkMaterial::Destroy()
{
	_pipeline->DeInit();
}

void VkMaterial::BindPipeline(VkCommandBuffer cmd, VkPipelineBindPoint inType)
{
	vkCmdBindPipeline(cmd, inType, _pipeline->GetPipeline());
}

bool VkMaterial::BindSet(VkCommandBuffer inBuffer, uint32_t inFrameNumber)
{
	vkCmdBindDescriptorSets(inBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GetPipelineLayout(), 2, 1, &_textureDescriptorSets[inFrameNumber], 0, nullptr);
	return true;
}


void VkMaterial::Update(VkDescriptorPool inPool)
{
	for (auto& descriptorSet : _textureDescriptorSets)
	{
		if (_vulkan->AllocateDescriptorSet(inPool, &descriptorSet, &_textureLayout))
		{
			fm::VkTexture* texture = _textures.front();
			VkDescriptorImageInfo imageInfo(texture->GetDescriptor());
			std::vector<VkWriteDescriptorSet> setWrites = {
				vk_init::CreateWriteDescriptorSet(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, descriptorSet, &imageInfo, 0)
			};
			vkUpdateDescriptorSets(_vulkan->GetDevice(), setWrites.size(), setWrites.data(), 0, nullptr);

			isReady = true;
		}
	}

}

VkPipelineLayout VkMaterial::GetPipelineLayout() const
{
	return _pipeline->GetPipelineLayout();
}

