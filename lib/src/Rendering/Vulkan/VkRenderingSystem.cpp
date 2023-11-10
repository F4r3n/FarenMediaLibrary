#include "Rendering/Vulkan/VkRenderingSystem.hpp"
#include "Rendering/Vulkan/Vulkan.hpp"
#include "Window.h"
#include "Resource/ResourcesManager.h"
#include "Rendering/Vulkan/VkShader.hpp"
#include "Rendering/Model.hpp"
#include "Rendering/StandardShapes.h"
#include "Rendering/Mesh.hpp"
#include "Rendering/Vulkan/VkVertexBuffer.hpp"
#include "Rendering/meshloader.hpp"
#include "Components/CCamera.h"
#include "VkModel.hpp"
#include "VkMaterial.hpp"
#include <ECS.h>
#include "Components/CMesh.h"
#include "Components/CMaterial.h"
#include "Components/CTransform.h"
#include "Components/CIdentity.h"
#include "VkTexture.hpp"
#include "Rendering/material.hpp"

using namespace fms;

VkRenderingSystem::VkRenderingSystem(std::shared_ptr<fm::Window> inWindow)
{
	_vulkan = std::make_unique<Vulkan>();
	_vulkan->Init(inWindow->getWindow());
	_window = inWindow;
	_type = SYSTEM_MODE::ALWAYS;
}

void VkRenderingSystem::init(EntityManager& manager, EventManager& event)
{
	_renderPass = _CreateRenderPass();
	_SetupDescriptors();
	_SetupGlobalUniforms();
	_SetupUploadContext();
	_vulkan->SetupSwapChainFramebuffer(_renderPass);
	_commandBuffers = _CreateCommandBuffers(_vulkan->GetCommandPool());
	_SetupSyncObjects();

	_InitStandardShapes();

	//TODO: TEST
	//fm::VkTexture texture();
	//texture.UploadImage(fm::FilePath(fm::LOCATION::INTERNAL_IMAGES_LOCATION, "lost_empire-RGBA.png"));
	//auto it = _textures.emplace(0, std::make_unique<fm::VkTexture>(_vulkan.get(), [this](std::function<void(VkCommandBuffer)> inCmd) {this->_ImmediateSubmit(std::move(inCmd)); }));
	//it.first->second->UploadImage(fm::FilePath(fm::LOCATION::INTERNAL_IMAGES_LOCATION, "lost_empire-RGBA.png"));
	//texture.Destroy();
	_textureCache = std::make_unique<fm::VkTextureCache>(_vulkan.get(), [this](std::function<void(VkCommandBuffer)> inCmd) {this->_ImmediateSubmit(std::move(inCmd)); });
}

bool VkRenderingSystem::_SetupUploadContext()
{
	VkFenceCreateInfo uploadFenceCreateInfo{};
	uploadFenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	uploadFenceCreateInfo.flags = 0;
	if (vkCreateFence(_vulkan->GetDevice(), &uploadFenceCreateInfo, nullptr, &_uploadContext._uploadFence) != VK_SUCCESS)
		return false;

	VkCommandPoolCreateInfo uploadCommandPoolInfo = vk_init::CreateCommandPoolCreateInfo(_vulkan->GetQueueFamilyIndices());
	if (vkCreateCommandPool(_vulkan->GetDevice(), &uploadCommandPoolInfo, nullptr, &_uploadContext._commandPool) != VK_SUCCESS)
		return false;

	VkCommandBufferAllocateInfo allocInfo = vk_init::CreateCommandBufferAllocateInfo(_uploadContext._commandPool);
	if (vkAllocateCommandBuffers(_vulkan->GetDevice(), &allocInfo, &_uploadContext._commandBuffer) != VK_SUCCESS) {
		return false;
	}

	return true;
}


void VkRenderingSystem::_InitStandardShapes()
{
	std::shared_ptr<fm::Model> quad = std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_MODELS_LOCATION, "Quad"));
	std::shared_ptr<fm::Model> quadFS = std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_MODELS_LOCATION, "QuadFS"));
	std::shared_ptr<fm::Model> circle = std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_MODELS_LOCATION, "Circle"));
	//std::shared_ptr<fm::Model> cube = std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_RESOURCES_LOCATION, "Cube"));
	quad->AddMesh(fm::StandardShapes::CreateQuad());
	circle->AddMesh(fm::StandardShapes::CreateCircle());
	quadFS->AddMesh(fm::StandardShapes::CreateQuadFullScreen());
	//cube->AddMesh(fm::StandardShapes::CreateTriangle(_api));
	fm::FilePath models(fm::ResourcesManager::GetFilePathResource(fm::LOCATION::INTERNAL_MODELS_LOCATION));
	fm::FilePath cubeModel = models.ToSub("cube.obj");
	std::shared_ptr<fm::Model> cube = fm::MeshLoader::Load(cubeModel, "Cube").value();
	fm::ResourcesManager::get().load<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_MODELS_LOCATION, "Cube"), cube);

	_staticModels.emplace(cube->GetID(), std::make_unique<fm::VkModel>(_vulkan.get(), cube));

	for (const auto& m : _staticModels)
	{
		_UploadMesh(m.second.get());
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
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 10 }
		,{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 10 }
		,{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 10 }
		,{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 10 }
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = 0;
	pool_info.maxSets = 10;
	pool_info.poolSizeCount = (uint32_t)sizes.size();
	pool_info.pPoolSizes = sizes.data();

	vkCreateDescriptorPool(_vulkan->GetDevice(), &pool_info, nullptr, &_descriptorPool);


	//information about the binding.
	VkDescriptorSetLayoutBinding camBufferBinding = vk_init::CreateDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
																							VK_SHADER_STAGE_VERTEX_BIT, 0);
	VkDescriptorSetLayoutBinding sceneBinding = vk_init::CreateDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
																							VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 1);

	_globalSetLayout = _vulkan->CreateDescriporSetLayout({ camBufferBinding,sceneBinding });
	if (_globalSetLayout == nullptr)
		return false;

	VkDescriptorSetLayoutBinding objectBinding = vk_init::CreateDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
																							VK_SHADER_STAGE_VERTEX_BIT, 2);
	_objectSetLayout = _vulkan->CreateDescriporSetLayout({ objectBinding });
	if (_objectSetLayout == nullptr)
		return false;

	//VkDescriptorSetLayoutBinding textureBinding = vk_init::CreateDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
	//	VK_SHADER_STAGE_FRAGMENT_BIT, 0);
	//_textureSetLayout = _vulkan->CreateDescriporSetLayout({ textureBinding });
	//if (_textureSetLayout == nullptr)
	//	return false;

	return true;
}

bool VkRenderingSystem::_SetupGlobalUniforms()
{
	const size_t sceneParamBufferSize = MAX_FRAMES_IN_FLIGHT * _vulkan->PadUniformBufferSize(sizeof(GPUSceneData));
	_sceneParameterBuffer = _vulkan->CreateBuffer(sceneParamBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		const int MAX_OBJECTS = 10000;
		_framesData[i].globalBuffer = _vulkan->CreateBuffer(sizeof(fmc::Shader_data), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
		_framesData[i].objectBuffer = _vulkan->CreateBuffer(sizeof(GPUObjectData) * MAX_OBJECTS, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);

		if (!_vulkan->AllocateDescriptorSet(_descriptorPool, &_framesData[i].globalDescriptorSet, &_globalSetLayout))
			return false;

		if (!_vulkan->AllocateDescriptorSet(_descriptorPool, &_framesData[i].objectDescriptor, &_objectSetLayout))
			return false;


		VkDescriptorBufferInfo binfo;
		binfo.buffer = _framesData[i].globalBuffer._buffer;
		binfo.offset = 0;
		binfo.range = sizeof(fmc::Shader_data);

		VkDescriptorBufferInfo sceneInfo;
		sceneInfo.buffer = _sceneParameterBuffer._buffer;
		sceneInfo.offset = 0;
		sceneInfo.range = sizeof(GPUSceneData);

		VkDescriptorBufferInfo objectInfo;
		objectInfo.buffer = _framesData[i].objectBuffer._buffer;
		objectInfo.offset = 0;
		objectInfo.range = sizeof(GPUObjectData)*MAX_OBJECTS;

		std::vector<VkWriteDescriptorSet> setWrites = {
			vk_init::CreateWriteDescriptorSet(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, _framesData[i].globalDescriptorSet, &binfo, 0),
			vk_init::CreateWriteDescriptorSet(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, _framesData[i].globalDescriptorSet, &sceneInfo, 1),
			vk_init::CreateWriteDescriptorSet(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, _framesData[i].objectDescriptor, &objectInfo, 2)
		};

		vkUpdateDescriptorSets(_vulkan->GetDevice(), (uint32_t)setWrites.size(), setWrites.data(), 0, nullptr);
	}

	return true;
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

std::shared_ptr<fm::VkShader> VkRenderingSystem::_FindOrCreateShader(const fm::SubShader& inSubShader)
{
	fm::ShaderID ID = inSubShader.GetID();
	auto it = _shaders.find(ID);
	if (it != _shaders.end())
		return it->second;

	std::shared_ptr<fm::VkShader> shader = std::make_shared<fm::VkShader>(inSubShader);
	_shaders.emplace(ID, shader);
	shader->Make(_vulkan->GetDevice());
	return shader;
}

bool VkRenderingSystem::_RecordCommandBuffer(VkCommandBuffer commandBuffer, VkFramebuffer inFrameBuffer, VkRenderPass inRenderPass, EntityManager& em)
{
	VkCommandBufferBeginInfo beginInfo = vk_init::CreateCommandBufferBeginInfo(0);

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		return false;
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = inRenderPass;
	renderPassInfo.framebuffer = inFrameBuffer;

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
	_vulkan->MapBuffer(_framesData[_currentFrame].globalBuffer, &camData, sizeof(fmc::Shader_data), 0);
	
	_sceneParameters.ambientColor = fm::math::vec4(1,1,1, 1);

	const size_t offset = _vulkan->PadUniformBufferSize(sizeof(GPUSceneData)) * _currentFrame;
	_vulkan->MapBuffer(_sceneParameterBuffer, &_sceneParameters, sizeof(GPUSceneData), offset);

	uint32_t uniform_offset = static_cast<uint32_t>(offset);

	_vulkan->MapBuffer(_framesData[_currentFrame].objectBuffer, [&em](void** inData) {
		GPUObjectData* objectSSBO = (GPUObjectData*)(*inData);
		size_t i = 0;
		for (auto&& e : em.iterate<fmc::CMesh, fmc::CMaterial, fmc::CTransform, fmc::CIdentity>())
		{
			fmc::CIdentity* identity = e.get<fmc::CIdentity>();
			if (!(identity == nullptr || identity->IsActive()))
				continue;

			fmc::CTransform* tranform = e.get<fmc::CTransform>();
			fm::math::mat model = tranform->GetWorldPosMatrix();
			objectSSBO[i].modelMatrix = model;
			i++;
		}
	});

	uint32_t instanceIndex = 0;

	//we can now draw the mesh
	for (auto&& e : em.iterate<fmc::CMesh, fmc::CMaterial, fmc::CTransform, fmc::CIdentity>())
	{
		fmc::CIdentity* identity = e.get<fmc::CIdentity>();
		if (!(identity == nullptr || identity->IsActive()))
			continue;

		fmc::CMaterial* material = e.get<fmc::CMaterial>();
		auto mainMaterial = material->GetMainMaterial();
		//
		auto itMaterial = _materials.find(mainMaterial->GetID());
		if (itMaterial != _materials.end())
		{
			if (_currentMaterial != itMaterial->second.get())
			{
				_currentMaterial = itMaterial->second.get();

				itMaterial->second->BindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS);
				vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _currentMaterial->GetPipelineLayout(), 0, 1, &_framesData[_currentFrame].globalDescriptorSet, 1, &uniform_offset);
				vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _currentMaterial->GetPipelineLayout(), 1, 1, &_framesData[_currentFrame].objectDescriptor, 0, nullptr);
				itMaterial->second->BindSet(commandBuffer, _currentFrame);
			}
		}
		else
		{
			fm::VkMaterial::VkMaterialCreateInfo materialInfo{};
			materialInfo.vulkan = _vulkan.get();
			materialInfo.renderPass = _renderPass;
			materialInfo.extent = _vulkan->GetSwapChainExtent();
			materialInfo.descriptorLayout = std::vector<VkDescriptorSetLayout>{ _globalSetLayout,_objectSetLayout };
			materialInfo.material = mainMaterial;
			//materialInfo.textureLayout = _textureSetLayout;
			//materialInfo.textures.push_back(_textures.find(0)->second.get());
			materialInfo.maxFramesInFlight = MAX_FRAMES_IN_FLIGHT;
			materialInfo.shader = _FindOrCreateShader(mainMaterial->GetSubShader().value());

			std::unique_ptr<fm::VkMaterial> mat = std::make_unique<fm::VkMaterial>(materialInfo);
			_materialsToUpdate.emplace_back(mat.get());

			_currentMaterial = _materials.emplace(mainMaterial->GetID(), std::move(mat)).first->second.get();

		}

		if (!_currentMaterial->IsReady())
			continue;

		fmc::CMesh* mesh = e.get<fmc::CMesh>();

		if (mesh->GetModel() == nullptr)
			continue;
		auto it = _staticModels.find(mesh->GetModel()->GetID());

		if (it != _staticModels.end())
		{
			it->second->Draw(commandBuffer, instanceIndex);
		}
		else
		{
			auto modelMesh = std::make_unique<fm::VkModel>(_vulkan.get(), mesh->GetModel());
			_UploadMesh(modelMesh.get());
			modelMesh->Draw(commandBuffer, instanceIndex);

			_staticModels.emplace(mesh->GetModel()->GetID(), std::move(modelMesh));
		}
		instanceIndex++;
	}

	_currentMaterial = nullptr;
	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		return false;
	}

	return true;
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

	for (auto& material : _materialsToUpdate)
	{
		material->Update(_descriptorPool, *_textureCache);
	}
	_materialsToUpdate.clear();


	vkWaitForFences(device, 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	if (!_vulkan->AcquireImage(_imageAvailableSemaphores[_currentFrame], imageIndex, _window->getWindow(), _renderPass))
	{
		return;
	}
	vkResetFences(device, 1, &_inFlightFences[_currentFrame]);

	vkResetCommandBuffer(_commandBuffers[_currentFrame], 0);
	_RecordCommandBuffer(_commandBuffers[_currentFrame], _vulkan->GetSwapChainFrameBuffer(imageIndex), _renderPass, manager);

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

void VkRenderingSystem::_UploadMesh(fm::VkModel* inModel)
{
	inModel->UploadData([this](std::function<void(VkCommandBuffer)> inCmd) {this->_ImmediateSubmit(std::move(inCmd)); });
}


void VkRenderingSystem::_ImmediateSubmit(std::function<void(VkCommandBuffer cmd)>&& function)
{
	VkCommandBuffer cmd = _uploadContext._commandBuffer;

	//begin the command buffer recording. We will use this command buffer exactly once before resetting, so we tell vulkan that
	VkCommandBufferBeginInfo cmdBeginInfo = vk_init::CreateCommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	vkBeginCommandBuffer(cmd, &cmdBeginInfo);

	//execute the function
	function(cmd);

	vkEndCommandBuffer(cmd);

	VkSubmitInfo submit = vk_init::CreateSubmitInfo(&cmd);


	//submit command buffer to the queue and execute it.
	// _uploadFence will now block until the graphic commands finish execution
	vkQueueSubmit(_vulkan->GetGraphicsQueue(), 1, &submit, _uploadContext._uploadFence);

	vkWaitForFences(_vulkan->GetDevice(), 1, &_uploadContext._uploadFence, true, 9999999999);
	vkResetFences(_vulkan->GetDevice(), 1, &_uploadContext._uploadFence);

	// reset the command buffers inside the command pool
	vkResetCommandPool(_vulkan->GetDevice(), _uploadContext._commandPool, 0);
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
	vkDestroyFence(_vulkan->GetDevice(), _uploadContext._uploadFence, nullptr);
	vkDestroyCommandPool(_vulkan->GetDevice(), _uploadContext._commandPool, nullptr);
	vkDestroyDescriptorSetLayout(_vulkan->GetDevice(), _globalSetLayout, nullptr);
	vkDestroyDescriptorSetLayout(_vulkan->GetDevice(), _objectSetLayout, nullptr);
	//vkDestroyDescriptorSetLayout(_vulkan->GetDevice(), _textureSetLayout, nullptr);

	vkDestroyDescriptorPool(_vulkan->GetDevice(), _descriptorPool, nullptr);
	for (const auto& [_, model] : _staticModels)
	{
		model->Destroy();
	}

	_textureCache->Destroy();

	for (const auto& [_, shader] : _shaders)
	{
		shader->Delete(_vulkan->GetDevice());
	}


	for (auto& frame : _framesData)
	{
		_vulkan->DestroyBuffer(frame.globalBuffer);
		_vulkan->DestroyBuffer(frame.objectBuffer);
	}

	_vulkan->DestroyBuffer(_sceneParameterBuffer);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(_vulkan->GetDevice(), _renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(_vulkan->GetDevice(), _imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(_vulkan->GetDevice(), _inFlightFences[i], nullptr);
	}

	for (const auto& [_, shader] : fm::ResourcesManager::get().getAll<fm::Shader>())
	{
		if (auto s = std::dynamic_pointer_cast<fm::VkShader>(shader))
		{
			s->Delete(_vulkan->GetDevice());
		}
	}

	for (const auto& [_, material] : _materials)
	{
		material->Destroy();
	}

	vkDestroyRenderPass(_vulkan->GetDevice(), _renderPass, nullptr);

	_vulkan->DeInit();
}