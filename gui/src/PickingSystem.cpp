#include "PickingSystem.h"
#include "Core/SceneManager.h"
#include "Core/GameObject.h"
#include "Components/CTransform.h"
#include "Components/CCamera.h"
#include "Window.h"
#include "Core/Scene.h"
using namespace fms;

PickingSystem::PickingSystem(std::function<void(fm::GameObject*)> &&inCallback)
{
	_specialCamera = fm::GameObjectHelper::create(fm::SceneManager::get().getScene("_editor"));
	_camera = _specialCamera->addComponent<fmc::CCamera>(fm::Window::kWidth, fm::Window::kHeight, fmc::RENDER_MODE::FORWARD, false, false, 4);

	_specialCamera->addComponent<fmc::CTransform>();
	_specialCamera->name = "Camera";
	_camera->target = std::make_shared<fm::RenderTexture>(fm::RenderTexture(_camera->getInternalRenderTexture(), 0));
	_callback = inCallback;

}


fm::GameObject* PickingSystem::PickGameObject(fm::GameObject* inCurrentCamera, fm::math::vec2 &inPos)
{
	std::shared_ptr<fm::Scene> scene = fm::SceneManager::get().getScene("_editor");
	_specialCamera->get<fmc::CTransform>()->From(inCurrentCamera->get<fmc::CTransform>());
	_camera->SetCallBackOnPostRendering([this]()
	{
		//Read pixel
		//Get id pixel from color
		//Get entity from id
		_callback(nullptr);
	});

	for (auto && go : scene->getAllGameObjects())
	{
		if (go->has<fmc::CTransform>() && go->has<fmc::CMesh>() && go->has<fmc::CMaterial>())
		{
			fmc::CMesh* mesh = go->get<fmc::CMesh>();
			fmc::CMaterial* materials = go->get<fmc::CMaterial>();

			fm::CommandBuffer commandBuffer;
			commandBuffer.DrawMesh(mesh->model, go->get<fmc::CTransform>()->GetTransform(), materials->GetMainMaterial());

			_camera->AddCommandBuffer(fm::RENDER_QUEUE::BEFORE_RENDERING_FILL_QUEUE, commandBuffer);
		}
	}
	return nullptr;
}
