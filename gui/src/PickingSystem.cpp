#include "PickingSystem.h"
#include "Core/SceneManager.h"
#include "Core/GameObject.h"
#include "Components/CTransform.h"
#include "Components/CCamera.h"
#include "Window.h"
#include "Core/Scene.h"
#include "Resource/ResourcesManager.h"
using namespace fms;

PickingSystem::PickingSystem( std::shared_ptr<fm::Scene> inEditorScene)
{
	_currentScene = fm::SceneManager::get().getCurrentScene();
	_editorScene = inEditorScene;
	_specialCamera = fm::GameObjectHelper::create(_editorScene);
	_camera = _specialCamera->addComponent<fmc::CCamera>(fm::Window::kWidth, fm::Window::kHeight, fmc::RENDER_MODE::FORWARD, false, false, 4);

	_specialCamera->addComponent<fmc::CTransform>();
	_specialCamera->name = "Camera";
	_camera->target = std::make_shared<fm::RenderTexture>(fm::RenderTexture(_camera->getInternalRenderTexture(), 0));

	fm::Shader* shader = fm::ResourcesManager::get().getResource<fm::Shader>("picking");

	_material = std::make_unique<fm::Material>("default_material");
	_material->shaderName = "picking";
	_material->shader = shader;
}


void PickingSystem::PickGameObject(ecs::id inCameraID, const fm::math::vec2 &inPos)
{
	fm::GameObject *cameraGo = _editorScene->GetGameObject(inCameraID);
	if (cameraGo != nullptr)
	{
		_specialCamera->get<fmc::CTransform>()->From(cameraGo->get<fmc::CTransform>());

		_camera->SetCallBackOnPostRendering([this, inPos]()
		{
			fm::Texture texture = _camera->target->GetColorBufferTexture(0);
			std::string path("C:\\Users\\guill\\Pictures\\yolo.png");
			texture.writeToPNG(path);
			//float id = texture.GetPixel<float>(inPos);
			//fm::GameObject* go = _editorScene->GetGameObject((size_t)id);
			//_callback(go);
		});

		for (auto && go : _currentScene->getAllGameObjects())
		{
			if (go->has<fmc::CTransform>() && go->has<fmc::CMesh>() && go->has<fmc::CMaterial>())
			{
				fmc::CMesh* mesh = go->get<fmc::CMesh>();

				fm::CommandBuffer commandBuffer;
				fm::MaterialProperties materialProperties = _material->GetProperties();
				materialProperties.AddValue("colorID", fm::MaterialValue((float)go->getID()));

				commandBuffer.DrawMesh(mesh->model, go->get<fmc::CTransform>()->GetTransform(), _material.get(), materialProperties);

				_camera->AddCommandBuffer(fm::RENDER_QUEUE::BEFORE_RENDERING_FILL_QUEUE, commandBuffer);
			}
		}
	}
	
}
