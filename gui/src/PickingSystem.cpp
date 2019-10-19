#include "PickingSystem.h"
#include "Core/SceneManager.h"
#include "Core/GameObject.h"
#include "Components/CTransform.h"
#include "Components/CCamera.h"
#include "Window.h"
#include "Core/Scene.h"
#include "Resource/ResourcesManager.h"
using namespace fms;

PickingSystem::PickingSystem(std::function<void(fm::GameObject*)> &&inCallback, std::shared_ptr<fm::Scene> inEditorScene)
{
	_editorScene = inEditorScene;
	_specialCamera = fm::GameObjectHelper::create(_editorScene);
	_camera = _specialCamera->addComponent<fmc::CCamera>(fm::Window::kWidth, fm::Window::kHeight, fmc::RENDER_MODE::FORWARD, false, false, 4);

	_specialCamera->addComponent<fmc::CTransform>();
	_specialCamera->name = "Camera";
	_camera->target = std::make_shared<fm::RenderTexture>(fm::RenderTexture(_camera->getInternalRenderTexture(), 0));
	_callback = inCallback;

	fm::Shader* shader = fm::ResourcesManager::get().getResource<fm::Shader>("picking");

	_material = std::make_unique<fm::Material>("default_material");
	_material->shaderName = "picking";
	_material->shader = shader;
}


fm::GameObject* PickingSystem::PickGameObject(fm::GameObject* inCurrentCamera, fm::math::vec2 &inPos)
{
	_specialCamera->get<fmc::CTransform>()->From(inCurrentCamera->get<fmc::CTransform>());
	_camera->SetCallBackOnPostRendering([this]()
	{
		fm::Texture texture = _camera->getInternalRenderTexture().GetColorBufferTexture(0);
		//texture.
		//Read pixel
		//Get id pixel from color
		//Get entity from id
		_callback(nullptr);
	});

	for (auto && go : _editorScene->getAllGameObjects())
	{
		if (go->has<fmc::CTransform>() && go->has<fmc::CMesh>() && go->has<fmc::CMaterial>())
		{
			fmc::CMesh* mesh = go->get<fmc::CMesh>();

			fm::CommandBuffer commandBuffer;
			_material->setValue("colorID", fm::MaterialValue((float)go->getID()));
			commandBuffer.DrawMesh(mesh->model, go->get<fmc::CTransform>()->GetTransform(), _material.get());

			_camera->AddCommandBuffer(fm::RENDER_QUEUE::BEFORE_RENDERING_FILL_QUEUE, commandBuffer);
		}
	}
	return nullptr;
}
