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
	if (auto renderTexture = _preview.renderTexture.lock())
	{
		if (renderTexture->isCreated())
		{
			const fm::OGLTexture texture = renderTexture->GetColorBufferTexture(0);
	
			ImGui::GetWindowDrawList()->AddImage(ImTextureID((intptr_t)texture.getID()), _startImagePos, _endImagePos);
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
				go->addComponent<fmc::CCamera>()->Init();
				
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
	if (auto renderTexture = _preview.renderTexture.lock())
	{
		bool isRenderTextureReady = false;


		isRenderTextureReady = renderTexture->isCreated();

		if (isRenderTextureReady && HasBeenDrawn())
		{
			fm::math::vec2 size;
			fm::math::vec2 start;
			fm::math::vec2 startCursorPos;

			const float rapport = (float)renderTexture->getWidth() / (float)renderTexture->getHeight();

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
	if (!camera->IsInit())
		camera->Init();
	if (camera->HasTarget() && camera->GetTarget()->isCreated())
	{
		_preview.renderTexture = camera->GetTarget();
	}
	else
	{
		_preview.renderTexture = camera->SetTarget();
	}

	fm::Debug::logErrorExit(glGetError(), __FILE__, __LINE__);
}

void GGameView::Clear()
{
	
}





void GGameView::Resize() {}
void GGameView::Zoom() {}
