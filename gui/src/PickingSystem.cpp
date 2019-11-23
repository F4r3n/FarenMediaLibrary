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
	_editorScene = inEditorScene;
	_specialCamera = fm::GameObjectHelper::create(_editorScene);
	_camera = _specialCamera->addComponent<fmc::CCamera>(fm::Window::kWidth, fm::Window::kHeight, fmc::RENDER_MODE::FORWARD, false, false, 0);
	_camera->Init();
	_specialCamera->addComponent<fmc::CTransform>();
	_specialCamera->name = "Camera";

#if 1
	fm::Format formats[] = { fm::Format::RGBA, fm::Format::RGBA, fm::Format::RGB };
	fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE };
	fm::RenderTexture renderTexture(_camera->getInternalRenderTexture().getWidth(),
		_camera->getInternalRenderTexture().getHeight(),
		3,
		formats,
		types,
		24,
		0);


	_camera->target = std::make_shared<fm::RenderTexture>(renderTexture);
#else
	_camera->target = std::make_shared<fm::RenderTexture>(_camera->getInternalRenderTexture(), 0);
#endif

	fm::Shader* shader = fm::ResourcesManager::get().getResource<fm::Shader>("picking");

	_material = std::make_unique<fm::Material>("default_material");
	_material->shaderName = "picking";
	_material->shader = shader;
}


void PickingSystem::PickGameObject(size_t inCameraID, const fm::math::vec2 &inPos)
{
	std::shared_ptr<fm::Scene> scene = fm::SceneManager::get().getCurrentScene();

	fm::GameObject *cameraGo = _editorScene->GetGameObject(inCameraID);
	if (cameraGo != nullptr)
	{
		_specialCamera->get<fmc::CTransform>()->From(cameraGo->get<fmc::CTransform>());

		_camera->SetCallBackOnPostRendering([this, inPos]()
		{
			std::shared_ptr<fm::Scene> scene = fm::SceneManager::get().getCurrentScene();

			fm::Texture texture = _camera->target->GetColorBufferTexture(0);
			
			unsigned char pixel[4];
			texture.GetPixel(inPos, pixel);
			size_t id = (pixel[0] + pixel[1] * 256 + pixel[2] * 256 * 256) - 1;
			fm::GameObject* go = scene->GetGameObject(id);
			_callback(go);
		});

		size_t i = 1;
		for (auto && go : scene->getAllGameObjects())
		{
			if (go->has<fmc::CTransform>() && go->has<fmc::CMesh>() && go->has<fmc::CMaterial>())
			{
				fmc::CMesh* mesh = go->get<fmc::CMesh>();

				fm::CommandBuffer commandBuffer;
				fm::MaterialProperties materialProperties = _material->GetProperties();
				unsigned char r[sizeof(i)];
				memcpy(r, &i, sizeof(i));
				

				//float colorID = go->getID();
				materialProperties.AddValue("colorID", fm::MaterialValue(fm::Color(r[0]/255.f, r[1]/255.f, r[2] / 255.f, r[3]/255.f)));
				commandBuffer.DrawMesh(mesh->model, go->get<fmc::CTransform>()->GetTransform(), _material.get(), materialProperties);

				_camera->AddCommandBuffer(fm::RENDER_QUEUE::BEFORE_RENDERING_FILL_QUEUE, commandBuffer);
			}
			++i;
		}
	}
	
}
