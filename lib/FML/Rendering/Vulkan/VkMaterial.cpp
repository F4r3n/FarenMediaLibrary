#include "VkMaterial.hpp"
#include "VkPipelineBuilder.hpp"
#include "Rendering/material.hpp"
#include "Rendering/Vulkan/VkTexture.hpp"
#include "Resource/ResourcesManager.h"
#include "Rendering/Vulkan/VkShader.hpp"
#include "Rendering/Vulkan/VkTextureCache.hpp"
using namespace fm;

VkMaterial::VkMaterial(const VkMaterialCreateInfo& inInfo)
{
	_material = inInfo.material;
	//_textureLayout = inInfo.textureLayout;
	_vulkan = inInfo.vulkan;

	fm::SubShader::Reflection r = inInfo.shader->GetReflection();
	auto layouts = inInfo.descriptorLayout;

	auto infoMaterialBuffer = inInfo.material->GetMaterialBufferInfo_Buffer(GRAPHIC_API::VULKAN);
	
	VkDescriptorSetLayoutBinding materialBinding = vk_init::CreateDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
		VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, infoMaterialBuffer.info.bindingPoint);
	_materialBuffer.bindingPoint = infoMaterialBuffer.info.bindingPoint;
	_materialBufferInfo.setPoint = infoMaterialBuffer.info.setPoint;
	_materialBufferInfo.frames.resize(inInfo.maxFramesInFlight);

	_materialBuffer.maxBufferSize = static_cast<uint32_t>(inInfo.maxFramesInFlight * _vulkan->PadUniformBufferSize(infoMaterialBuffer.bufferSize));
	_materialBuffer.buffer = _vulkan->CreateBuffer(_materialBuffer.maxBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

	_materialBufferInfo.layout = _vulkan->CreateDescriporSetLayout({ materialBinding });
	layouts.emplace_back(_materialBufferInfo.layout);

	auto infoMaterialTextures = inInfo.material->GetMaterialBufferInfo_Textures(GRAPHIC_API::VULKAN);
	std::vector< VkDescriptorSetLayoutBinding> bindings;
	for (const auto& infoTexture : infoMaterialTextures)
	{
		MaterialBufferTexture matTexture;
		bindings.emplace_back(vk_init::CreateDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			VK_SHADER_STAGE_FRAGMENT_BIT, infoTexture.info.bindingPoint));
		matTexture.bindingPoint = infoTexture.info.bindingPoint;

		matTexture.texture = infoTexture.texture;

		_materialBufferTextures.emplace_back(matTexture);
	}
	_materialBufferTextureInfo.frames.resize(1);

	if (!bindings.empty())
	{
		_materialBufferTextureInfo.layout = _vulkan->CreateDescriporSetLayout(bindings);
		_materialBufferTextureInfo.setPoint = infoMaterialTextures.front().info.setPoint;
		layouts.emplace_back(_materialBufferTextureInfo.layout);
	}


	_pipeline = std::make_unique<fm::VkPipelineBuilder>(_vulkan->GetDevice(), inInfo.renderPass, inInfo.extent, layouts, inInfo.shader);
}

bool VkMaterial::IsReady() const
{
	for (const auto& r : _materialBufferInfo.frames)
	{
		if (!r.descriptorSetReady)
			return false;
	}

	if (_materialBufferTextureInfo.layout != nullptr)
	{
		for (const auto& r : _materialBufferTextureInfo.frames)
		{
			if (!r.descriptorSetReady)
				return false;
		}
	}

	return true;
}



void VkMaterial::Destroy()
{

	_vulkan->DestroyBuffer(_materialBuffer.buffer);
	vkDestroyDescriptorSetLayout(_vulkan->GetDevice(), _materialBufferInfo.layout, nullptr);
	vkDestroyDescriptorSetLayout(_vulkan->GetDevice(), _materialBufferTextureInfo.layout, nullptr);

	_pipeline->DeInit();
}

void VkMaterial::BindPipeline(VkCommandBuffer cmd, VkPipelineBindPoint inType)
{
	vkCmdBindPipeline(cmd, inType, _pipeline->GetPipeline());
}

bool VkMaterial::BindSet(VkCommandBuffer inBuffer, uint32_t inFrameNumber)
{
	if (_materialBufferInfo.frames[inFrameNumber].descriptorSetReady)
	{
		uint32_t offset = 0;
		if (_materialBufferInfo.frames[inFrameNumber].stamp != _material->GetStamp())
		{
			_materialBufferInfo.frames[inFrameNumber].stamp = _material->GetStamp();
			offset = static_cast<uint32_t>(_vulkan->PadUniformBufferSize(_material->GetBufferSize()) * inFrameNumber);
			_vulkan->MapBuffer(_materialBuffer.buffer, _material->GetBufferPtr(), (uint32_t)_material->GetBufferSize(), offset);
		}

		vkCmdBindDescriptorSets(inBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GetPipelineLayout(), _materialBufferInfo.setPoint, 1,
			&_materialBufferInfo.frames[inFrameNumber].descriptorSet, 1, &offset);
	}

	if (_materialBufferTextureInfo.layout != nullptr)
	{
		if (_materialBufferTextureInfo.GetFrame(inFrameNumber).descriptorSetReady)
		{
			vkCmdBindDescriptorSets(inBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GetPipelineLayout(),
				_materialBufferTextureInfo.setPoint,
				static_cast<uint32_t>(_materialBufferTextures.size()),
				&_materialBufferTextureInfo.GetFrame(inFrameNumber).descriptorSet, 0, nullptr);
		}

	}
	return true;
}


void VkMaterial::Update(VkDescriptorPool inPool, VkTextureCache& inCache)
{
	for (auto& frame : _materialBufferInfo.frames)
	{
		if (_vulkan->AllocateDescriptorSet(inPool, &frame.descriptorSet, &_materialBufferInfo.layout))
		{
			VkDescriptorBufferInfo materialInfo;
			materialInfo.buffer = _materialBuffer.buffer._buffer;
			materialInfo.offset = 0;
			materialInfo.range = _material->GetBufferSize();


			std::vector<VkWriteDescriptorSet> setWrites = {
			vk_init::CreateWriteDescriptorSet(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, frame.descriptorSet, &materialInfo, _materialBuffer.bindingPoint)
			};
			vkUpdateDescriptorSets(_vulkan->GetDevice(), (uint32_t)setWrites.size(), setWrites.data(), 0, nullptr);
			frame.descriptorSetReady = true;
		}
	}


	for (auto& materialTexture : _materialBufferTextures)
	{
		if (auto tex = materialTexture.texture.lock())
		{
			materialTexture.vkTexture = inCache.FindOrCreateTexture(tex);
		}
	}

	if (_materialBufferTextureInfo.layout != nullptr)
	{
		for (auto& frame : _materialBufferTextureInfo.frames)
		{
			if (_vulkan->AllocateDescriptorSet(inPool, &frame.descriptorSet, &_materialBufferTextureInfo.layout))
			{
				std::vector<VkWriteDescriptorSet> setWrites;

				for (const auto& materialTexture : _materialBufferTextures)
				{
					std::shared_ptr<fm::VkTexture> currentTexture = materialTexture.vkTexture == nullptr ? inCache._blankTexture : materialTexture.vkTexture;
					VkDescriptorImageInfo imageInfo(currentTexture->GetDescriptor());
					setWrites.push_back(vk_init::CreateWriteDescriptorSet(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, frame.descriptorSet, &imageInfo, materialTexture.bindingPoint));
				}
				frame.descriptorSetReady = true;

				vkUpdateDescriptorSets(_vulkan->GetDevice(), static_cast<uint32_t>(setWrites.size()), setWrites.data(), 0, nullptr);

			}
		}
	}

}

VkPipelineLayout VkMaterial::GetPipelineLayout() const
{
	return _pipeline->GetPipelineLayout();
}

