#include "Rendering/Vulkan/VkRenderingSystem.h"
#include "Rendering/Vulkan/Vulkan.h"
#include "Window.h"
#include "Resource/ResourcesManager.h"
#include "Rendering/Vulkan/VkShader.h"
#include "Rendering/Model.hpp"
#include "Rendering/StandardShapes.h"
#include "Rendering/Mesh.hpp"
#include "Rendering/Vulkan/VkVertexBuffer.hpp"
#include "Rendering/meshloader.hpp"
#include "VkModel.h"
const int MAX_FRAMES_IN_FLIGHT = 2;

using namespace fms;

VkRenderingSystem::VkRenderingSystem(std::shared_ptr<fm::Window> inWindow)
{
	_vulkan = std::make_unique<Vulkan>();
	_vulkan->Init(inWindow->getWindow());
	_window = inWindow;
	fm::ResourcesManager::get().LoadShaders(_api);
	//fm::ResourcesManager::get().LoadFonts();
	fm::ResourcesManager::get().LoadMaterials();


	_renderPass = _CreateRenderPass();
	_pipeline = fm::VkPipelineBuilder(_vulkan->GetDevice(), _renderPass, _vulkan->GetSwapChainExtent(),
		fm::ResourcesManager::get().getResource<fm::Shader>("default").get());
	_vulkan->SetupSwapChainFramebuffer(_renderPass);
	_commandBuffers = _CreateCommandBuffers(_vulkan->GetCommandPool());
	_SetupSyncObjects();

	 fm::File models(fm::File(fm::ResourcesManager::GetFilePathResource(fm::LOCATION::INTERNAL_MODELS_LOCATION).ToSubFile("monkey_smooth.obj")));

	 _modelToDrawTest = fm::MeshLoader::Load(models.GetPath(), "monkey").value();
	 auto a = std::make_unique<fm::VkModel>(_vulkan->GetAllocator(), _modelToDrawTest);
	 a->UploadData();
	 _modelsUploaded.emplace(_modelToDrawTest->GetID(), std::move(a));

}

bool VkRenderingSystem::_SetupSyncObjects()
{
	_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(_vulkan->GetDevice(), &semaphoreInfo, nullptr, &_imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(_vulkan->GetDevice(), &semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(_vulkan->GetDevice(), &fenceInfo, nullptr, &_inFlightFences[i]) != VK_SUCCESS) {

			return false;
		}
	}
	return true;
}


VkRenderPass VkRenderingSystem::_CreateRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = _vulkan->GetSwapChainFormat();
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depthAttachment = {};
	// Depth attachment
	depthAttachment.flags = 0;
	depthAttachment.format = _vulkan->GetDepthFormat();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depth_attachment_ref = {};
	depth_attachment_ref.attachment = 1;
	depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depth_attachment_ref;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcAccessMask = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	VkAttachmentDescription attachments[2] = { colorAttachment, depthAttachment };
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 2;
	renderPassInfo.pAttachments = &attachments[0];
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;



	VkRenderPass renderPass;
	if (vkCreateRenderPass(_vulkan->GetDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
		return nullptr;
	}
	return renderPass;
}


std::vector<VkCommandBuffer> VkRenderingSystem::_CreateCommandBuffers(VkCommandPool inPool)
{
	std::vector<VkCommandBuffer> commandBuffers(MAX_FRAMES_IN_FLIGHT);
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = inPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(_vulkan->GetDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
	return commandBuffers;
}

bool VkRenderingSystem::_RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VkRenderPass inRenderPass, VkPipeline inPipeline)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		return false;
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = inRenderPass;
	renderPassInfo.framebuffer = _vulkan->GetSwapChainFrameBuffer(imageIndex);

	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = _vulkan->GetSwapChainExtent();


	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = {{0.0f, 0.0f, 1.0f, 1.0f}};
	clearValues[1].depthStencil = {1.0f, 0};
	renderPassInfo.clearValueCount = clearValues.size();
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, inPipeline);
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(_vulkan->GetSwapChainExtent().width);
	viewport.height = static_cast<float>(_vulkan->GetSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = _vulkan->GetSwapChainExtent();
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	//bind the mesh vertex buffer with offset 0

	fm::VkShader::MeshPushConstants constants;

	fm::math::vec3 camPos = { 0.f,0.f,-2.0f };

	fm::math::mat view = fm::math::translate(fm::math::mat(1.f), camPos);
	fm::math::mat projection = fm::math::perspective(fm::math::radians(70.f), ((float)_vulkan->GetSwapChainExtent().width / (float)_vulkan->GetSwapChainExtent().height), 0.1f, 200.0f);
	projection[1][1] *= -1;
	fm::math::mat model = fm::math::rotate(fm::math::mat{ 1.0f }, fm::math::radians(3.14f), fm::math::vec3(0, 1, 0));
	fm::math::mat mesh_matrix = projection * view * model;

	constants.data = fm::math::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	constants.render_matrix = mesh_matrix;
	
	//constants.render_matrix.identity();
	//upload the matrix to the GPU via push constants
	vkCmdPushConstants(commandBuffer, _pipeline.GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(fm::VkShader::MeshPushConstants), &constants);

	//we can now draw the mesh
	auto it = _modelsUploaded.find(_modelToDrawTest->GetID());
	if (it != _modelsUploaded.end())
	{
		it->second->Draw(commandBuffer);
	}
	else
	{
		auto model = std::make_unique<fm::VkModel>(_vulkan->GetAllocator(), _modelToDrawTest);
		model->UploadData();
		model->Draw(commandBuffer);

		_modelsUploaded.emplace(_modelToDrawTest->GetID(), std::move(model));
	}

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		return false;
	}

	return true;
}


void VkRenderingSystem::init(EntityManager& manager, EventManager& event)
{

}
void VkRenderingSystem::pre_update(EntityManager& manager)
{

}
void VkRenderingSystem::update(float dt, EntityManager& manager, EventManager& event)
{
	if (_window->IsMinimized())
		return;

	auto device = _vulkan->GetDevice();
	vkWaitForFences(device, 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	if (!_vulkan->AcquireImage(_imageAvailableSemaphores[_currentFrame], imageIndex, _window->getWindow(), _renderPass))
	{
		return;
	}
	vkResetFences(device, 1, &_inFlightFences[_currentFrame]);

	vkResetCommandBuffer(_commandBuffers[_currentFrame], 0);
	_RecordCommandBuffer(_commandBuffers[_currentFrame], imageIndex, _renderPass, _pipeline.GetPipeline());

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { _imageAvailableSemaphores[_currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &_commandBuffers[_currentFrame];

	VkSemaphore signalSemaphores[] = { _renderFinishedSemaphores[_currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(_vulkan->GetGraphicsQueue(), 1, &submitInfo, _inFlightFences[_currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	_vulkan->SubmitPresentQueue(signalSemaphores, imageIndex, _window->getWindow(), _renderPass);

	_currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

}
void VkRenderingSystem::over()
{

}
void VkRenderingSystem::Start()
{

}
void VkRenderingSystem::Stop()
{

}
VkRenderingSystem::~VkRenderingSystem()
{
	vkDeviceWaitIdle(_vulkan->GetDevice());
	for (const auto& [_, model] : _modelsUploaded)
	{
		model->Destroy();
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(_vulkan->GetDevice(), _renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(_vulkan->GetDevice(), _imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(_vulkan->GetDevice(), _inFlightFences[i], nullptr);
	}

	std::shared_ptr<fm::VkShader> shader = std::dynamic_pointer_cast<fm::VkShader>(fm::ResourcesManager::get().getResource<fm::Shader>("default"));
	shader->Delete(_vulkan->GetDevice());

	_pipeline.DeInit();
	vkDestroyRenderPass(_vulkan->GetDevice(), _renderPass, nullptr);

	_vulkan->DeInit();
}