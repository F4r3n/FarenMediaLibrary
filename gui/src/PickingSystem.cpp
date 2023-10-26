#include "PickingSystem.h"
#include "Core/SceneManager.h"
#include "Core/GameObject.h"
#include "Components/CTransform.h"
#include "Components/CCamera.h"
#include "Window.h"
#include "Core/Scene.h"
#include "Resource/ResourcesManager.h"
#include "Core/application.h"
#include "Components/CMaterial.h"
#include "Components/CMesh.h"
#include "Rendering/Shader.h"
#include "Rendering/material.hpp"
#include "Rendering/commandBuffer.hpp"
using namespace fms;

PickingSystem::PickingSystem( std::shared_ptr<fm::Scene> inEditorScene)
{
	_editorScene = inEditorScene;
	_specialCamera = inEditorScene->CreateGameObject(true);
	if (auto&& specialCamera = _specialCamera.lock())
	{
		auto size = fm::Application::Get().GetWindow()->GetSize();

		_camera = specialCamera->addComponent<fmc::CCamera>(size.x, size.y, fmc::RENDER_MODE::FORWARD, false, false, 0);
		_camera->Init();
		specialCamera->SetName("Camera");
	}


	fm::Format formats[] = { fm::Format::RGBA, fm::Format::RGBA, fm::Format::RGB };
	fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE };

	_camera->SetTarget(fm::RenderTexture(_camera->getInternalRenderTexture().getWidth(),
		_camera->getInternalRenderTexture().getHeight(),
		3,
		formats,
		types,
		24,
		0));


	_material = std::make_shared<fm::Material>(fm::FilePath(fm::LOCATION::INTERNAL_MATERIALS_LOCATION,"default"));
	_material->SetShaderPath(fm::FilePath(fm::LOCATION::INTERNAL_SHADERS_LOCATION, "picking.shader"));
	//_material->Compile();
}


void PickingSystem::PickGameObject(const std::string &inSceneName, size_t inCameraID, const fm::math::vec2 &inPos)
{
	std::weak_ptr<fm::Scene> scene = fm::Application::Get().GetScene(inSceneName);

	if(auto && s = scene.lock())
	{
		std::shared_ptr<fm::GameObject> cameraGo = nullptr;
		if(auto && editorScene = _editorScene.lock())
		{
			cameraGo = editorScene->GetGameObjectByID(EntityManager::get().CreateID(inCameraID));
			if (cameraGo != nullptr)
			{
				if (auto&& specialCamera = _specialCamera.lock())
				{
					specialCamera->get<fmc::CTransform>()->From(cameraGo->get<fmc::CTransform>());
				}
				
				_camera->SetCallBackOnPostRendering([this, inPos, inSceneName]()
					{
						std::shared_ptr<fm::Scene> scene = fm::Application::Get().GetScene(inSceneName);
						if (scene != nullptr)
						{
							fm::OGLTexture texture = _camera->GetTarget()->GetColorBufferTexture(0);
							_camera->GetTarget()->bind(true);
							unsigned char pixel[4];
							texture.GetPixel(inPos, pixel);
							size_t id = ((size_t)pixel[0] + pixel[1] * 256 + pixel[2] * 256 * 256);
							std::shared_ptr<fm::GameObject> go = scene->GetGameObjectByID(EntityManager::get().CreateID(id));
							if (go != nullptr)
							{
								_callback(go->getID());
							}
						}
					});

				{
					fm::CommandBuffer commandBuffer;
					commandBuffer.Clear(fm::BUFFER_BIT::COLOR_BUFFER_BIT | fm::BUFFER_BIT::DEPTH_BUFFER_BIT);
					_camera->AddCommandBuffer(fm::RENDER_QUEUE::FIRST_STATE, commandBuffer);
				}

				for (auto&& o : s->GetAllGameObjects())
				{
					std::shared_ptr<fm::GameObject> go = o.second;
					if (go->has<fmc::CTransform>() && go->has<fmc::CMesh>() && go->has<fmc::CMaterial>())
					{
						fmc::CMesh* mesh = go->get<fmc::CMesh>();

						fm::CommandBuffer commandBuffer;
						fm::MaterialProperties materialProperties = _material->GetUniforms();
						Entity::Id i = go->getID();
						unsigned char r[sizeof(i)];
						memcpy(r, &i, sizeof(i));


						//float colorID = go->getID();
						materialProperties.AddValue("colorID", fm::MaterialValue(fm::Color(r[0] / 255.f, r[1] / 255.f, r[2] / 255.f, r[3] / 255.f)));
						commandBuffer.DrawMesh(mesh->model, go->get<fmc::CTransform>()->GetTransform(), _material, materialProperties);

						_camera->AddCommandBuffer(fm::RENDER_QUEUE::BEFORE_RENDERING_FILL_QUEUE, commandBuffer);
					}
				}
			}
		}
	}
	
}
