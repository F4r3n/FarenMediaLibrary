#include "VkMaterial.hpp"
#include "VkPipelineBuilder.hpp"
#include "Rendering/material.hpp"
#include "Rendering/Vulkan/VkTexture.hpp"
#include "Resource/ResourcesManager.h"
#include "Rendering/Vulkan/VkShader.hpp"
using namespace fm;

VkMaterial::VkMaterial(const VkMaterialCreateInfo& inInfo)
{
	_textures = inInfo.textures;
	_material = inInfo.material;
	//_textureLayout = inInfo.textureLayout;
	_vulkan = inInfo.vulkan;
	_textureDescriptorSets.resize(inInfo.maxFramesInFlight);

	fm::SubShader::Reflection r = inInfo.shader->GetReflection();
	auto layouts = inInfo.descriptorLayout;

	auto infoMaterialBuffer = inInfo.material->GetMaterialBufferInfo(GRAPHIC_API::VULKAN);
	
	VkDescriptorSetLayoutBinding materialBinding = vk_init::CreateDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
		VK_SHADER_STAGE_VERTEX_BIT, infoMaterialBuffer.bindingPoint);
	_materialBuffer.bindingPoint = infoMaterialBuffer.bindingPoint;
	_materialBuffer.setPoint = infoMaterialBuffer.setPoint;

	_materialBuffer.bufferLayout = _vulkan->CreateDescriporSetLayout({ materialBinding });
	_materialBuffer.maxBufferSize = static_cast<uint32_t>(inInfo.maxFramesInFlight * _vulkan->PadUniformBufferSize(infoMaterialBuffer.bufferSize));
	_materialBuffer.buffer = _vulkan->CreateBuffer(_materialBuffer.maxBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
	_materialBuffer.descriptorSets.resize(inInfo.maxFramesInFlight);
	_materialBuffer.stamps.resize(inInfo.maxFramesInFlight);
	isReady = _materialBuffer.descriptorSetReady;
	layouts.emplace_back(_materialBuffer.bufferLayout);

	//_vulkan->

	//if (!_textures.empty()) //The texture cannot be set during rendering, should wait for next frame
	//{
	//	layouts.emplace_back(_textureLayout);
	//	isReady = false;
	//}
	
	_pipeline = std::make_unique<fm::VkPipelineBuilder>(_vulkan->GetDevice(), inInfo.renderPass, inInfo.extent, layouts, inInfo.shader);
}


void VkMaterial::Destroy()
{

	_vulkan->DestroyBuffer(_materialBuffer.buffer);
	vkDestroyDescriptorSetLayout(_vulkan->GetDevice(), _materialBuffer.bufferLayout, nullptr);

	_pipeline->DeInit();
}

void VkMaterial::BindPipeline(VkCommandBuffer cmd, VkPipelineBindPoint inType)
{
	vkCmdBindPipeline(cmd, inType, _pipeline->GetPipeline());
}

bool VkMaterial::BindSet(VkCommandBuffer inBuffer, uint32_t inFrameNumber)
{
	if (_materialBuffer.descriptorSetReady)
	{
		uint32_t offset = 0;
		if (_materialBuffer.stamps[inFrameNumber] != _material->GetStamp())
		{
			_materialBuffer.stamps[inFrameNumber] = _material->GetStamp();
			offset = static_cast<uint32_t>(_vulkan->PadUniformBufferSize(_material->GetBufferSize()) * inFrameNumber);
			_vulkan->MapBuffer(_materialBuffer.buffer, _material->GetBufferPtr(), (uint32_t)_material->GetBufferSize(), offset);
		}

		vkCmdBindDescriptorSets(inBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GetPipelineLayout(), _materialBuffer.setPoint, 1,
			&_materialBuffer.descriptorSets[inFrameNumber], 1, &offset);
	}


	//vkCmdBindDescriptorSets(inBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GetPipelineLayout(), 2, 1, &_textureDescriptorSets[inFrameNumber], 0, nullptr);
	return true;
}


void VkMaterial::Update(VkDescriptorPool inPool)
{
	for (auto& descriptor : _materialBuffer.descriptorSets)
	{
		if (_vulkan->AllocateDescriptorSet(inPool, &descriptor, &_materialBuffer.bufferLayout))
		{
			VkDescriptorBufferInfo materialInfo;
			materialInfo.buffer = _materialBuffer.buffer._buffer;
			materialInfo.offset = 0;
			materialInfo.range = _material->GetBufferSize();


			std::vector<VkWriteDescriptorSet> setWrites = {
			vk_init::CreateWriteDescriptorSet(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, descriptor, &materialInfo, _materialBuffer.bindingPoint)
			};
			vkUpdateDescriptorSets(_vulkan->GetDevice(), (uint32_t)setWrites.size(), setWrites.data(), 0, nullptr);
		}
	}
	_materialBuffer.descriptorSetReady = true;
	isReady = _materialBuffer.descriptorSetReady;

	//isReady = true;
	//for (auto& descriptorSet : _textureDescriptorSets)
	//{
	//	if (_vulkan->AllocateDescriptorSet(inPool, &descriptorSet, &_textureLayout))
	//	{
	//		fm::VkTexture* texture = _textures.front();
	//		VkDescriptorImageInfo imageInfo(texture->GetDescriptor());
	//		std::vector<VkWriteDescriptorSet> setWrites = {
	//			vk_init::CreateWriteDescriptorSet(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, descriptorSet, &imageInfo, 0)
	//		};
	//		vkUpdateDescriptorSets(_vulkan->GetDevice(), setWrites.size(), setWrites.data(), 0, nullptr);
	//	
	//		isReady = true;
	//	}
	//}

}

VkPipelineLayout VkMaterial::GetPipelineLayout() const
{
	return _pipeline->GetPipelineLayout();
}

