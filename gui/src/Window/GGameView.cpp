#include "GGameView.h"
#include "Components/CCamera.h"
#include "Core/SceneManager.h"
#include "PickingSystem.h"
#include "ImGuizmo/ImGuizmo.h"
#include "Input/InputManager.h"
#include <imgui/imgui_internal.h>
#include "Core/Scene.h"
#include "Core/Debug.h"
#include "Core/application.h"
#include "Components/CTransform.h"
#include "Rendering/OpenGL/OGLTexture.hpp"
#include "Rendering/OpenGL/OGLFrameBuffer.hpp"
using namespace gui;
GGameView::GGameView() : GWindow("Game View", true, ImGuiWindowFlags_HorizontalScrollbar)
{
	_enabled = true;
	_aspectMode = ASPECT_MODE::ASPECT_16_9;
	_kind = gui::WINDOWS::WIN_SCENE_VIEW;

}

GGameView::~GGameView() 
{
}

void GGameView::CustomDraw()
{
	if (auto renderTexture = _preview.renderTexture)
	{
		if (renderTexture->IsCreated())
		{
			std::shared_ptr<fm::OGLTexture> texture = std::dynamic_pointer_cast<fm::OGLFrameBuffer>(renderTexture)->GetColorBufferTexture(0);
	
			ImGui::GetWindowDrawList()->AddImage(ImTextureID((intptr_t)texture->getID()), _startImagePos, _endImagePos);
		}
	}
	else
	{
		if (ImGui::Button("No camera found"))
		{
			AddEvent([](GWindow* window, std::optional<Context> inContext) {
				if (!inContext.has_value())
					return;
				auto scene = fm::Application::Get().GetScene(inContext->currentSceneName);
				auto go = scene->CreateGameObject(true);
				auto tr = go->get<fmc::CTransform>();
				tr->SetPosition(fm::math::vec3(0, 0, -1));
				go->addComponent<fmc::CCamera>();
				
				go->SetName("Main camera");
				dynamic_cast<GGameView*>(window)->AddCamera(go);
				
			});
		}
	}
}

void GGameView::BeforeWindowCreation()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
}
void GGameView::AfterWindowCreation()
{
	ImGui::PopStyleVar(1);
}


void GGameView::_Update(float dt, Context &inContext)
{
	if (auto renderTexture = _preview.renderTexture)
	{
		bool isRenderTextureReady = false;


		isRenderTextureReady = renderTexture->IsCreated();

		if (isRenderTextureReady && HasBeenDrawn())
		{
			fm::math::vec2 size;
			fm::math::vec2 start;
			fm::math::vec2 startCursorPos;

			const float rapport = (float)renderTexture->GetWidth() / (float)renderTexture->GetHeight();

			start = GetPosition();
			size = GetSize();
			ImVec2 end;
			if (_aspectMode == ASPECT_MODE::ASPECT_FREE)
			{
				end.x = start.x + size.x;
				end.y = start.y + size.y;
			}
			else if (_aspectMode == ASPECT_MODE::ASPECT_16_9)
			{
				end.x = start.x + size.x;
				end.y = start.y + size.x * 9.0f / 16.0f;
			}

			_startImagePos.x = start.x;
			_startImagePos.y = start.y;


			_endImagePos = fm::math::vec2(end.x, end.y);

			if (_aspectMode == ASPECT_MODE::ASPECT_16_9)
			{
				fm::math::vec2 offset;
				offset.x = 0;
				offset.y = size.y * 0.5f - (end.y - start.y) * 0.5f;
				_startImagePos += offset;
				_endImagePos += offset;
			}


		}
	}
}


void GGameView::AddCamera(std::shared_ptr<fm::GameObject> inGameObject)
{
	fmc::CCamera *camera = inGameObject->get<fmc::CCamera>();
	_preview.renderTexture = std::make_shared<fm::OGLFrameBuffer>(*fm::CreateRenderTexture(camera->GetWidth(), camera->GetHeight(), 0));
	camera->SetTarget(_preview.renderTexture);

	fm::Debug::logErrorExit(glGetError(), __FILE__, __LINE__);
}

void GGameView::Clear()
{
	
}


void GGameView::OnInit()
{

}

void GGameView::WillClose()
{

}


void GGameView::Resize() {}
void GGameView::Zoom() {}
