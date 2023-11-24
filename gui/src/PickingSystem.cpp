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
#include "Rendering/OpenGL/OGLTexture.hpp"
#include <cstring>
#include "Rendering/TextureDef.hpp"
#include "Rendering/OpenGL/OGLFrameBuffer.hpp"
#include "Components/CIdentity.h"
using namespace fms;

PickingSystem::PickingSystem( std::shared_ptr<fm::Scene> inEditorScene)
{
	_editorScene = inEditorScene;
	_specialCamera = inEditorScene->CreateGameObject(true);
	if (auto&& specialCamera = _specialCamera.lock())
	{
		auto size = fm::Application::Get().GetWindow()->GetSize();

		fmc::CCamera &camera = specialCamera->addComponent<fmc::CCamera>(size.x, size.y, fmc::RENDER_MODE::FORWARD, false, false, 0);
		specialCamera->SetName("Camera");

		std::vector<fm::TextureFormat> formats{ fm::TextureFormat::RGBA, fm::TextureFormat::RGBA, fm::TextureFormat::RGB };
		std::vector<fm::TextureType> types{ fm::TextureType::UNSIGNED_BYTE, fm::TextureType::UNSIGNED_BYTE, fm::TextureType::UNSIGNED_BYTE };
		camera.SetTarget(std::make_shared<fm::OGLFrameBuffer>(camera.GetWidth(), camera.GetHeight(), formats, types, 24, 0));
	}

	_material = std::make_shared<fm::Material>(fm::FilePath(fm::LOCATION::INTERNAL_MATERIALS_LOCATION,"default"));
	_material->SetShaderPath(fm::FilePath(fm::LOCATION::INTERNAL_SHADERS_LOCATION, "picking.shader"));
	//_material->Compile();
}


void PickingSystem::PickGameObject(const std::string &inSceneName, fm::GameObjectID_t inCameraID, const fm::math::vec2 &inPos, std::function<void(fm::GameObjectID_t)> inCallback)
{
	std::weak_ptr<fm::Scene> scene = fm::Application::Get().GetScene(inSceneName);

	if(auto && s = scene.lock())
	{
		std::shared_ptr<fm::GameObject> cameraGo = nullptr;
		if(auto && editorScene = _editorScene.lock())
		{
			cameraGo = editorScene->GetGameObjectByID(inCameraID);
			if (cameraGo != nullptr)
			{
				if (auto&& specialCamera = _specialCamera.lock())
				{
					specialCamera->get<fmc::CTransform>().From(cameraGo->get<fmc::CTransform>());
				
					fmc::CCamera& camera = specialCamera->get<fmc::CCamera>();
					camera.SetCallBackOnPostRendering([this, &camera, inPos, inSceneName, inCallback]()
						{
							std::shared_ptr<fm::Scene> scene = fm::Application::Get().GetScene(inSceneName);
							if (scene != nullptr)
							{
								std::shared_ptr<fm::OGLFrameBuffer> oglFrameBuffer = std::dynamic_pointer_cast<fm::OGLFrameBuffer>(camera.GetTarget());
								std::shared_ptr<fm::OGLTexture> texture = oglFrameBuffer->GetColorBufferTexture(0);
								oglFrameBuffer->bind(true);
								unsigned char pixel[4];
								texture->GetPixel(inPos, pixel);
								uint32_t id = ((uint32_t)pixel[0] + pixel[1] * 256 + pixel[2] * 256 * 256);
								std::shared_ptr<fm::GameObject> go = scene->GetGameObjectByID(id);
								if (go != nullptr)
								{
									if (inCallback != nullptr)
									{
										inCallback(go->GetID());
									}
									else
									{
										_callback(go->GetID());
									}
								}
							}
						});


					{
						fm::CommandBuffer commandBuffer;
						commandBuffer.Clear(fm::BUFFER_BIT::COLOR_BUFFER_BIT | fm::BUFFER_BIT::DEPTH_BUFFER_BIT);
						camera.AddCommandBuffer(fm::RENDER_QUEUE_FIRST_STATE, commandBuffer);
					}

					for (auto&& o : s->GetAllGameObjects())
					{
						std::shared_ptr<fm::GameObject> go = o.second;
						if (go->has<fmc::CTransform>() && go->has<fmc::CMesh>() && go->has<fmc::CMaterial>())
						{
							fmc::CIdentity* identity = go->try_get<fmc::CIdentity>();
							if (identity != nullptr)
							{
								if (!identity->IsActive())
									continue;
							}
							fmc::CMesh& mesh = go->get<fmc::CMesh>();

							fm::CommandBuffer commandBuffer;
							fm::MaterialValues materialProperties = _material->GetUniforms();
							const fm::GameObjectID_t i = go->GetID();
							unsigned char r[sizeof(i)];
							memcpy(r, &i, sizeof(i));

							//float colorID = go->getID();
							materialProperties["FM_M"] = go->get<fmc::CTransform>().GetTransform().worldTransform;
							materialProperties["colorID"] = fm::MaterialValue(fm::Color(r[0] / 255.f, r[1] / 255.f, r[2] / 255.f, r[3] / 255.f));
							commandBuffer.DrawMesh(mesh.GetModel(), go->get<fmc::CTransform>().GetTransform(), _material, materialProperties);

							camera.AddCommandBuffer(fm::RENDER_QUEUE_BEFORE_RENDERING_FILL_QUEUE, commandBuffer);
						}
					}
				}

			}
		}
	}
	
}
