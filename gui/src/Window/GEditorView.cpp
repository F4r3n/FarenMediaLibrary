#include "GEditorView.h"
#include "Components/CCamera.h"
#include "Core/Scene.h"
#include "Core/SceneManager.h"
#include "PickingSystem.h"
#include "ImGuizmo/ImGuizmo.h"
#include "Input/InputManager.h"
#include <imgui/imgui_internal.h>
#include "Core/application.h"
#include "Components/CTransform.h"
#include "Components/CMesh.h"
#include "Components/CMaterial.h"
#include "Rendering/Graphics.hpp"

using namespace gui;
GEditorView::GEditorView(std::shared_ptr<fm::GameObject> inCamera, std::shared_ptr<fm::Scene> inScene) :
	GWindow("Editor View", true, ImGuiWindowFlags_HorizontalScrollbar)
{
	_editorScene = inScene;
	_enabled = true;

	_pickingSystem = nullptr;
	_resultPicking = false;

	if (inCamera != nullptr)
	{
		_editorView.id = inCamera->getID();
		fmc::CCamera *camera = inCamera->get<fmc::CCamera>();
		_editorView.renderTexture = camera->SetTarget();
	}

	_kind = gui::WINDOWS::WIN_EDITOR_VIEW;

}

void GEditorView::_DrawContentEditorCamera(Context &inContext)
{
	if (!_editorView.id.has_value()) return;

	if (auto&& editorScene = _editorScene.lock())
	{
		std::shared_ptr<fm::GameObject> camera = editorScene->GetGameObjectByID(_editorView.id.value());
		std::shared_ptr<fm::Scene> currentScene = fm::Application::Get().GetScene(inContext.currentSceneName);
		if (camera != nullptr && camera->IsActive() && currentScene != nullptr)
		{
			fm::Scene::MapOfGameObjects gos = currentScene->GetAllGameObjects();
			{
				fm::CommandBuffer commandBuffer;
				commandBuffer.Clear(fm::BUFFER_BIT::COLOR_BUFFER_BIT | fm::BUFFER_BIT::DEPTH_BUFFER_BIT);
				camera->get<fmc::CCamera>()->AddCommandBuffer(fm::RENDER_QUEUE::FIRST_STATE, commandBuffer);
			}

			{
				fm::CommandBuffer commandBuffer;
				commandBuffer.Enable(fm::RENDERING_TYPE::DEPTH_TEST);
				camera->get<fmc::CCamera>()->AddCommandBuffer(fm::RENDER_QUEUE::FIRST_STATE, commandBuffer);
			}
			for (auto&& o : gos)
			{
				std::shared_ptr<fm::GameObject> go = o.second;
				if (!go->IsActive())
					continue;
				if (go->has<fmc::CTransform>() && go->has<fmc::CMesh>() && go->has<fmc::CMaterial>())
				{
					fmc::CMesh* mesh = go->get<fmc::CMesh>();

					fm::CommandBuffer commandBuffer;
					fm::MaterialProperties materialProperties;

					commandBuffer.DrawMesh(mesh->model, go->get<fmc::CTransform>()->GetTransform(), go->get<fmc::CMaterial>()->GetMainMaterial(), materialProperties);

					camera->get<fmc::CCamera>()->AddCommandBuffer(fm::RENDER_QUEUE::BEFORE_RENDERING_FILL_QUEUE, commandBuffer);
				}
			}
		}
	}
}


GEditorView::~GEditorView()
{
}

void GEditorView::CustomDraw()
{

	if (auto && renderTexture = _editorView.renderTexture.lock())
	{
		if (renderTexture->isCreated())
		{
			const fm::Texture texture = renderTexture->GetColorBufferTexture(0);

			ImGui::SetCursorPos(_cursorPos);
			ImGui::Image((ImTextureID)texture.getID(), ImVec2(texture.getWidth(), texture.getHeight()));
			ImGuizmo::SetDrawlist();
			_scrollPos = fm::math::vec2(ImGui::GetScrollX(), ImGui::GetScrollY());

			_EditObject();
		}
	}
    
}

void GEditorView::BeforeWindowCreation()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
}
void GEditorView::AfterWindowCreation()
{
	ImGui::PopStyleVar(1);
}

void GEditorView::_EditObject()
{
	if (!_editorView.id.has_value())
		return;

	if (!_gameObjectSelectedByPicking.has_value())
		return;

	if (auto&& editorScene = _editorScene.lock())
	{
		std::shared_ptr<fm::GameObject> camera = editorScene->GetGameObjectByID(_editorView.id.value());

		ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
		if (_currentTransformContext == gui::TRANSFORM_CONTEXT::TRANSLATE)
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		else if (_currentTransformContext == gui::TRANSFORM_CONTEXT::ROTATE)
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		else if (_currentTransformContext == gui::TRANSFORM_CONTEXT::SCALE)
			mCurrentGizmoOperation = ImGuizmo::SCALE;

		std::shared_ptr<fm::GameObject> go = fm::Application::Get().GetCurrentScene()->GetGameObjectByID(_gameObjectSelectedByPicking.value());
		if (go != nullptr)
		{
			if (auto&& renderTexture = _editorView.renderTexture.lock())
			{
				fmc::CTransform* transform = go->get<fmc::CTransform>();
				//delete transform;
				ImGuiIO& io = ImGui::GetIO();
				//float scrollPosX = ImGui::GetScrollX();
				ImGuizmo::SetRect(_cursorPos.x + _startImagePos.x - ImGui::GetScrollX(),
					_cursorPos.y + _startImagePos.y - ImGui::GetScrollY(),
					renderTexture->getWidth(),
					renderTexture->getHeight());
				const fm::math::mat view = camera->get<fmc::CTransform>()->GetLocalMatrixModel();
				const fm::math::mat projecttion = camera->get<fmc::CCamera>()->GetProjectionMatrix();
				fm::math::mat model;

				fm::math::vec3 rotation = transform->GetRotation().GetEulerAngles();
				fm::math::vec3 position = transform->GetPosition();
				fm::math::vec3 scale = transform->GetScale();
				ImGuizmo::RecomposeMatrixFromComponents(&position[0], &rotation[0], &scale[0], &model[0][0]);

				ImGuizmo::Manipulate(&view[0][0], &projecttion[0][0], mCurrentGizmoOperation, mCurrentGizmoMode, &model[0][0], NULL, NULL);

				ImGuizmo::DecomposeMatrixToComponents(&model[0][0], &position[0], &rotation[0], &scale[0]);
				transform->SetPosition(position);
				transform->SetScale(scale);
				transform->SetRotation(fm::math::Quaternion::FromEulerAngles(rotation));
			}
		}
	}
		
}


void GEditorView::_CallBackPickingSystem(ecs::id inID)
{
	_resultPicking = true;
	_gameObjectSelectedByPicking = inID;
}


void GEditorView::SetPickingSystem(std::unique_ptr<fms::PickingSystem> inPickingSystem)
{
	_pickingSystem = std::move(inPickingSystem);
	std::function<void(ecs::id inID)> f = std::bind(&gui::GEditorView::_CallBackPickingSystem, this, std::placeholders::_1);
	_pickingSystem->SetCallback(std::move(f));
}


void GEditorView::_Update(float dt, Context &inContext)
{
	_DrawContentEditorCamera(inContext);

	//assert(GImGui != nullptr && GImGui->CurrentWindow != nullptr);
	if (auto&& renderTexture = _editorView.renderTexture.lock())
	{
		if (renderTexture->isCreated() && HasBeenDrawn())
		{
			fm::math::vec2 startCursorPos;

			const float rapport = (float)renderTexture->getWidth() / (float)renderTexture->getHeight();

			fm::math::vec2 start = GetPosition();
			fm::math::vec2 size = GetSize();
			fm::math::vec2 end(start.x + size.x, start.y + size.y);

			_startImagePos.x = start.x;
			_startImagePos.y = start.y;

			_cursorPos = fm::math::vec2(-(renderTexture->getWidth() - size.x) * 0.5f,
				-(renderTexture->getHeight() - size.y) * 0.5f);
			startCursorPos = _cursorPos;
			startCursorPos.y += start.y;
			startCursorPos.x += start.x;

			_endImagePos = fm::math::vec2(end.x, end.y);

			if (ImGui::IsMouseReleased(0) && !ImGuizmo::IsUsing())
			{
				fm::math::vec2 mousePos = ImGui::GetMousePos();

				fm::Rectf rect;
				rect.w = size.x;
				rect.h = size.y;
				rect.x = start.x;
				rect.y = start.y;


				if (rect.contains(mousePos))
				{
					fm::math::vec2 mPos(mousePos);
					mPos.x -= startCursorPos.x - _scrollPos.x;
					mPos.y -= startCursorPos.y - _scrollPos.y;

					_pickingSystem->PickGameObject(inContext.currentSceneName, _editorView.id.value(), mPos);
				}
			}


			if (_resultPicking)
			{
				inContext.currentGameObjectSelected = _gameObjectSelectedByPicking;
				_resultPicking = false;
			}
			_gameObjectSelectedByPicking = inContext.currentGameObjectSelected;

		}
	}

	_currentTransformContext = inContext.currentTransformContext;
}



void GEditorView::Resize() {}
void GEditorView::Zoom() {}
