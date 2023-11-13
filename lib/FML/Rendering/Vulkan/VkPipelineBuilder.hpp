#pragma once
#include "vulkan/vulkan.h"
#include <vector>
#include <memory>
namespace fm
{
	class Shader;
	class VkShader;
	class VkPipelineBuilder
	{
	public:
		VkPipelineBuilder(VkDevice inDevice, VkRenderPass inRenderPass, VkExtent2D inExtent, const std::vector<VkDescriptorSetLayout>& inDescriptorLayouts,
			std::shared_ptr<VkShader> inShader);
		~VkPipelineBuilder();
		VkPipelineBuilder() = default;

		VkPipeline			GetPipeline() const { return _pipeline; }
		VkPipelineLayout	GetPipelineLayout() const { return _pipelineLayout; }

		void DeInit();
	private:
		VkPipelineRasterizationStateCreateInfo _CreateRasterizerInfo() const;
		VkPipelineColorBlendStateCreateInfo		_CreateColorBlendingInfo(VkPipelineColorBlendAttachmentState* inAttachmentState) const;
		VkPipelineColorBlendAttachmentState		_CreateColorBlendingAttachmentInfo() const;
		VkPipelineDepthStencilStateCreateInfo	_CreateDepthStencilInfo() const;
		VkPipelineMultisampleStateCreateInfo	_CreateMultisampleInfo() const;

		VkPipelineLayout	_CreatePipelineLayout(const std::vector<VkDescriptorSetLayout>& inDescriptorLayouts);
		VkPipeline			_CreatePipeline(VkPipelineLayout inLayout,
			VkRenderPass inRenderPass,
			VkExtent2D inExtent,
			VkShader* inShader);

	private:
		VkDevice _device = nullptr;

		VkPipelineLayout _pipelineLayout = nullptr;
		VkPipeline		 _pipeline = nullptr;
	};

}