#include "Rendering/Vulkan/VkRenderingSystem.h"
#include "Rendering/Vulkan/Vulkan.h"
#include "Window.h"
#include "Resource/ResourcesManager.h"
#include "Rendering/Vulkan/VkShader.h"
#include "Rendering/Model.hpp"
#include "Rendering/StandardShapes.h"
#include "Rendering/Mesh.hpp"
#include "Rendering/Vulkan/VkVertexBuffer.hpp"
const int MAX_FRAMES_IN_FLIGHT = 2;

using namespace fms;

VkRenderingSystem::VkRenderingSystem(std::shared_ptr<fm::Window> inWindow)
{
	_vulkan = std::make_unique<Vulkan>();
	_vulkan->Init(inWindow->getWindow());
	_window = inWindow;
	fm::ResourcesManager::get().LoadShaders(GRAPHIC_API::VULKAN);
	//fm::ResourcesManager::get().LoadFonts();
	fm::ResourcesManager::get().LoadMaterials();


	_renderPass = _CreateRenderPass();
	_pipeline = fm::VkPipelineBuilder(_vulkan->GetDevice(), _renderPass, _vulkan->GetSwapChainExtent(),
		fm::ResourcesManager::get().getResource<fm::Shader>("default"));
	_vulkan->SetupSwapChainFramebuffer(_renderPass);
	_commandBuffers = _CreateCommandBuffers(_vulkan->GetCommandPool());
	_SetupSyncObjects();

	fm::rendering::MeshContainer* triangle = fm::StandardShapes::CreateTriangle(GRAPHIC_API::VULKAN);
	 _triangle = fm::VkVertexBuffer(_vulkan->GetAllocator());
	 _triangle.generate(triangle->vertices);
	
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

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
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

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

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
	VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &_triangle._allocatedBuffer._buffer, &offset);

	//we can now draw the mesh
	vkCmdDraw(commandBuffer, _triangle.GetNumberVertices(), 1, 0, 0);

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
	vkWaitForFences(device, 1, &_inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	if (!_vulkan->AcquireImage(_imageAvailableSemaphores[currentFrame], imageIndex, _window->getWindow(), _renderPass))
	{
		return;
	}
	vkResetFences(device, 1, &_inFlightFences[currentFrame]);

	vkResetCommandBuffer(_commandBuffers[currentFrame], 0);
	_RecordCommandBuffer(_commandBuffers[currentFrame], imageIndex, _renderPass, _pipeline.GetPipeline());

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { _imageAvailableSemaphores[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &_commandBuffers[currentFrame];

	VkSemaphore signalSemaphores[] = { _renderFinishedSemaphores[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(_vulkan->GetGraphicsQueue(), 1, &submitInfo, _inFlightFences[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	_vulkan->SubmitPresentQueue(signalSemaphores, imageIndex, _window->getWindow(), _renderPass);

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

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
	_triangle.destroy();

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(_vulkan->GetDevice(), _renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(_vulkan->GetDevice(), _imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(_vulkan->GetDevice(), _inFlightFences[i], nullptr);
	}

	fm::VkShader* shader = dynamic_cast<fm::VkShader*>(fm::ResourcesManager::get().getResource<fm::Shader>("default"));
	shader->Delete(_vulkan->GetDevice());

	_pipeline.DeInit();
	vkDestroyRenderPass(_vulkan->GetDevice(), _renderPass, nullptr);

	_vulkan->DeInit();
}