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
#include "Components/CCamera.h"
#include "VkModel.h"
#include "VkMaterial.h"
#include <ECS.h>
#include "Components/CMesh.h"
#include "Components/CMaterial.h"
#include "Components/CTransform.h"
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
	_SetupDescriptors();
	_SetupGlobalUniforms();
	_InitStandardShapes();

	_vulkan->SetupSwapChainFramebuffer(_renderPass);
	_commandBuffers = _CreateCommandBuffers(_vulkan->GetCommandPool());
	_SetupSyncObjects();

}

void VkRenderingSystem::_InitStandardShapes()
{
	std::shared_ptr<fm::Model> quad = std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_RESOURCES_LOCATION, "Quad"));
	std::shared_ptr<fm::Model> quadFS = std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_RESOURCES_LOCATION, "QuadFS"));
	std::shared_ptr<fm::Model> circle = std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_RESOURCES_LOCATION, "Circle"));
	//std::shared_ptr<fm::Model> cube = std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_RESOURCES_LOCATION, "Cube"));
	quad->AddMesh(fm::StandardShapes::CreateQuad());
	circle->AddMesh(fm::StandardShapes::CreateCircle());
	quadFS->AddMesh(fm::StandardShapes::CreateQuadFullScreen());
	//cube->AddMesh(fm::StandardShapes::CreateTriangle(_api));
	fm::FilePath models(fm::ResourcesManager::GetFilePathResource(fm::LOCATION::INTERNAL_MODELS_LOCATION));

	fm::FilePath cubeModel = models.ToSub("cube.obj");
	std::shared_ptr<fm::Model> cube = fm::MeshLoader::Load(cubeModel, "Cube").value();
	//cube->AddMesh(fm::StandardShapes::CreateCube());


	//fm::ResourcesManager::get().load<fm::Model>(quad->GetName(), quad);
	//fm::ResourcesManager::get().load<fm::Model>(quadFS->GetName(), quadFS);
	//fm::ResourcesManager::get().load<fm::Model>(circle->GetName(), circle);
	fm::ResourcesManager::get().load<fm::Model>(cube->GetName(), cube);
	//_staticModels.emplace(quad->GetID(), std::make_unique<fm::VkModel>(_vulkan->GetAllocator(), quad));
	_staticModels.emplace(cube->GetID(), std::make_unique<fm::VkModel>(_vulkan->GetAllocator(), cube));
	//_staticModels.emplace(quadFS->GetID(), std::make_unique<fm::VkModel>(_vulkan->GetAllocator(), quadFS));
	//_staticModels.emplace(circle->GetID(), std::make_unique<fm::VkModel>(_vulkan->GetAllocator(), circle));

	for (const auto& m : _staticModels)
	{
		m.second->UploadData();
	}
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

bool VkRenderingSystem::_SetupDescriptors()
{
	std::vector<VkDescriptorPoolSize> sizes =
	{
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 10 },
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = 0;
	pool_info.maxSets = 10;
	pool_info.poolSizeCount = (uint32_t)sizes.size();
	pool_info.pPoolSizes = sizes.data();

	vkCreateDescriptorPool(_vulkan->GetDevice(), &pool_info, nullptr, &_descriptorPool);


	//information about the binding.
	VkDescriptorSetLayoutBinding camBufferBinding = _vulkan->CreateDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
	VkDescriptorSetLayoutBinding sceneBinding = _vulkan->CreateDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
		VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 1);

	VkDescriptorSetLayoutBinding bindings[] = { camBufferBinding,sceneBinding };
	VkDescriptorSetLayoutCreateInfo setinfo = {};
	setinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	setinfo.pNext = nullptr;
	setinfo.bindingCount = 2;
	setinfo.flags = 0;
	setinfo.pBindings = bindings;

	const VkResult result = vkCreateDescriptorSetLayout(_vulkan->GetDevice(), &setinfo, nullptr, &_globalSetLayout);

	return result == VK_SUCCESS;
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

bool VkRenderingSystem::_RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VkRenderPass inRenderPass, EntityManager& em)
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
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

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


	fm::math::vec3 camPos = { 0.f,0.f,-2.0f };

	fm::math::mat view = fm::math::translate(fm::math::mat(1.f), camPos);
	fm::math::mat projection = fm::math::perspective(fm::math::radians(70.f), ((float)_vulkan->GetSwapChainExtent().width / (float)_vulkan->GetSwapChainExtent().height), 0.1f, 200.0f);
	projection[1][1] *= -1;

	fmc::Shader_data camData;
	camData.FM_P = projection;
	camData.FM_V = view;
	camData.FM_PV = projection * view;
	//_vulkan->MapBuffer(_globalBuffers[_currentFrame], &camData, sizeof(fmc::Shader_data));
	_vulkan->MapBuffer(_framesData[_currentFrame].globalBuffer, &camData, sizeof(fmc::Shader_data), 0);
	
	float framed = (_frame / 120.f);
	_sceneParameters.ambientColor = { sin(framed),0,cos(framed),1 };

	const size_t offset = _vulkan->pad_uniform_buffer_size(sizeof(GPUSceneData)) * _currentFrame;
	_vulkan->MapBuffer(_sceneParameterBuffer, &_sceneParameters, sizeof(GPUSceneData), offset);

	uint32_t uniform_offset = offset;

	//we can now draw the mesh
	for (auto&& e : em.iterate<fmc::CMesh, fmc::CMaterial, fmc::CTransform>())
	{
		fmc::CMaterial* material = e.get<fmc::CMaterial>();
		auto mainMaterial = material->GetMainMaterial();
		//
		auto itMaterial = _materials.find(mainMaterial->GetID());
		if (itMaterial != _materials.end())
		{
			if (_currentMaterial != itMaterial->second.get())
			{
				itMaterial->second->BindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS);
				_currentMaterial = itMaterial->second.get();

				vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _currentMaterial->GetPipelineLayout(), 0, 1, &_framesData[_currentFrame].globalDescriptorSet, 1, &uniform_offset);
			}
		}
		else
		{
			std::unique_ptr<fm::VkMaterial> mat = std::make_unique<fm::VkMaterial>(mainMaterial, _vulkan->GetDevice(), _renderPass, _vulkan->GetSwapChainExtent(), _globalSetLayout);
			_currentMaterial = _materials.emplace(material->GetID(), std::move(mat)).first->second.get();
			_currentMaterial->BindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS);
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _currentMaterial->GetPipelineLayout(), 0, 1, &_framesData[_currentFrame].globalDescriptorSet, 1, &uniform_offset);
		}

		fmc::CTransform* tranform = e.get<fmc::CTransform>();
		fm::math::mat model = tranform->GetWorldPosMatrix();
		fm::VkShader::MeshPushConstants constants;
		constants.render_matrix = model;
		vkCmdPushConstants(commandBuffer, _currentMaterial->GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(fm::VkShader::MeshPushConstants), &constants);

		fmc::CMesh* mesh = e.get<fmc::CMesh>();
		if (mesh->model == nullptr)
		{
			mesh->model = fm::ResourcesManager::get().getResource<fm::Model>(mesh->GetModelType());
		}
		if (mesh->model == nullptr)
			continue;
		auto it = _staticModels.find(mesh->model->GetID());

		if (it != _staticModels.end())
		{
			it->second->Draw(commandBuffer);
		}
		else
		{
			auto modelMesh = std::make_unique<fm::VkModel>(_vulkan->GetAllocator(), mesh->model);
			modelMesh->UploadData();
			modelMesh->Draw(commandBuffer);

			_staticModels.emplace(mesh->model->GetID(), std::move(modelMesh));
		}
	}

	_currentMaterial = nullptr;
	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		return false;
	}

	return true;
}

bool VkRenderingSystem::_SetupGlobalUniforms()
{
	const size_t sceneParamBufferSize = MAX_FRAMES_IN_FLIGHT * _vulkan->pad_uniform_buffer_size(sizeof(GPUSceneData));
	_sceneParameterBuffer = _vulkan->CreateBuffer(sceneParamBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		_framesData[i].globalBuffer = _vulkan->CreateBuffer(sizeof(fmc::Shader_data), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.pNext = nullptr;
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = _descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &_globalSetLayout;

		VkResult result = vkAllocateDescriptorSets(_vulkan->GetDevice(), &allocInfo, &_framesData[i].globalDescriptorSet);
		if (result != VkResult::VK_SUCCESS)
			return false;

		VkDescriptorBufferInfo binfo;
		binfo.buffer = _framesData[i].globalBuffer._buffer;
		binfo.offset = 0;
		binfo.range = sizeof(fmc::Shader_data);

		VkDescriptorBufferInfo sceneInfo;
		sceneInfo.buffer = _sceneParameterBuffer._buffer;
		sceneInfo.offset = 0;
		sceneInfo.range = sizeof(GPUSceneData);

		VkWriteDescriptorSet cameraWrite = _vulkan->CreateWriteDescriptorSet(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, _framesData[i].globalDescriptorSet, &binfo, 0);
		VkWriteDescriptorSet sceneWrite = _vulkan->CreateWriteDescriptorSet(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, _framesData[i].globalDescriptorSet, &sceneInfo, 1);

		VkWriteDescriptorSet setWrites[] = { cameraWrite,sceneWrite };

		vkUpdateDescriptorSets(_vulkan->GetDevice(), 2, setWrites, 0, nullptr);
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
	_frame++;
	auto device = _vulkan->GetDevice();
	vkWaitForFences(device, 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	if (!_vulkan->AcquireImage(_imageAvailableSemaphores[_currentFrame], imageIndex, _window->getWindow(), _renderPass))
	{
		return;
	}
	vkResetFences(device, 1, &_inFlightFences[_currentFrame]);

	vkResetCommandBuffer(_commandBuffers[_currentFrame], 0);
	_RecordCommandBuffer(_commandBuffers[_currentFrame], imageIndex, _renderPass, manager);

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
	vkDestroyDescriptorSetLayout(_vulkan->GetDevice(), _globalSetLayout, nullptr);
	vkDestroyDescriptorPool(_vulkan->GetDevice(), _descriptorPool, nullptr);
	for (const auto& [_, model] : _staticModels)
	{
		model->Destroy();
	}

	for (auto& frame : _framesData)
	{
		_vulkan->DestroyBuffer(frame.globalBuffer);
	}
	_vulkan->DestroyBuffer(_sceneParameterBuffer);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(_vulkan->GetDevice(), _renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(_vulkan->GetDevice(), _imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(_vulkan->GetDevice(), _inFlightFences[i], nullptr);
	}

	std::shared_ptr<fm::VkShader> shader = std::dynamic_pointer_cast<fm::VkShader>(fm::ResourcesManager::get().getResource<fm::Shader>("default"));
	shader->Delete(_vulkan->GetDevice());

	for (const auto& [_, material] : _materials)
	{
		material->Destroy();
	}

	vkDestroyRenderPass(_vulkan->GetDevice(), _renderPass, nullptr);

	_vulkan->DeInit();
}