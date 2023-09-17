#include "VkPipelineBuilder.h"
#include "Rendering/Shader.h"
#include "Rendering/Vulkan/VkShader.h"
#include "Rendering/Vulkan/VkVertexBuffer.hpp"
using namespace fm;

VkPipelineBuilder::VkPipelineBuilder(VkDevice inDevice, VkRenderPass inRenderPass, VkExtent2D inExtent,
	const std::vector<VkDescriptorSetLayout>& inDescriptorLayouts,
	Shader* inShader)
{
	_device = inDevice;
	_pipelineLayout = _CreatePipelineLayout(inDescriptorLayouts);
	_pipeline = _CreatePipeline(_pipelineLayout, inRenderPass, inExtent, dynamic_cast<VkShader*>(inShader));
}

VkPipelineBuilder::~VkPipelineBuilder()
{
}

VkPipelineLayout VkPipelineBuilder::_CreatePipelineLayout(const std::vector<VkDescriptorSetLayout>& inDescriptorLayouts)
{

	VkPipelineLayout pipelineLayout;
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = inDescriptorLayouts.size();
	pipelineLayoutInfo.pSetLayouts = inDescriptorLayouts.data();

	//setup push constants
	VkPushConstantRange push_constant;
	push_constant.offset = 0;
	push_constant.size = sizeof(VkShader::MeshPushConstants);


	//this push constant range is accessible only in the vertex shader
	push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	pipelineLayoutInfo.pPushConstantRanges = &push_constant;
	pipelineLayoutInfo.pushConstantRangeCount = 1;

	if (vkCreatePipelineLayout(_device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		return nullptr;
	}
	return pipelineLayout;
}


VkPipeline  VkPipelineBuilder::_CreatePipeline(VkPipelineLayout inLayout,
	VkRenderPass inRenderPass,
	VkExtent2D inExtent,
	VkShader* inShader)
{
	inShader->Make(_device);

	//TODO: Change for Vertex
	VkVertexInputBindingDescription bindingDesc = fm::VkVertexBuffer::GetBindingDescription();
	 auto attributeDesc = fm::VkVertexBuffer::GetAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDesc; // Optional
	vertexInputInfo.vertexAttributeDescriptionCount = attributeDesc.size();
	vertexInputInfo.pVertexAttributeDescriptions = attributeDesc.data(); // Optional

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	//Viewport
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)inExtent.width;
	viewport.height = (float)inExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = inExtent;

	std::vector<VkDynamicState> dynamicStates = {
	VK_DYNAMIC_STATE_VIEWPORT,
	VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	//Rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizer = _CreateRasterizerInfo();

	//Multisampling
	VkPipelineMultisampleStateCreateInfo multisampling = _CreateMultisampleInfo();

	//Depth
	VkPipelineDepthStencilStateCreateInfo depthStencil = _CreateDepthStencilInfo();

	//Color blending (no blending)
	VkPipelineColorBlendAttachmentState colorBlendAttachment =	_CreateColorBlendingAttachmentInfo();
	VkPipelineColorBlendStateCreateInfo colorBlending =			_CreateColorBlendingInfo(&colorBlendAttachment);

	VkPipelineShaderStageCreateInfo shaderStages[] = { inShader->GetStageCreateInfo(fm::VkShader::KIND::VERT),
												   inShader->GetStageCreateInfo(fm::VkShader::KIND::FRAG) };

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = inLayout;
	pipelineInfo.renderPass = inRenderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional
	pipelineInfo.pDepthStencilState = &depthStencil;

	VkPipeline pipeline;
	if (vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
		return nullptr;
	}
	return pipeline;
}

VkPipelineMultisampleStateCreateInfo VkPipelineBuilder::_CreateMultisampleInfo() const
{
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	return multisampling;
}


VkPipelineDepthStencilStateCreateInfo VkPipelineBuilder::_CreateDepthStencilInfo() const
{
	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; // Optional
	depthStencil.back = {}; // Optional

	return depthStencil;
}


VkPipelineColorBlendAttachmentState	VkPipelineBuilder::_CreateColorBlendingAttachmentInfo() const
{
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	return colorBlendAttachment;
}

VkPipelineRasterizationStateCreateInfo VkPipelineBuilder::_CreateRasterizerInfo() const
{
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_NONE;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	return rasterizer;
}



VkPipelineColorBlendStateCreateInfo VkPipelineBuilder::_CreateColorBlendingInfo(VkPipelineColorBlendAttachmentState* inAttachmentState ) const
{


	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = inAttachmentState;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	return colorBlending;
}


void VkPipelineBuilder::DeInit()
{
	vkDestroyPipeline(_device, _pipeline, nullptr);
	vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);
}
