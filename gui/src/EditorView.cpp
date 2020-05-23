#include "EditorView.h"
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
EditorView::EditorView(fm::GameObject* inCamera, std::shared_ptr<fm::Scene> inScene) : GWindow("Editor View", true, ImGuiWindowFlags_HorizontalScrollbar
												)
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
		_editorView.enabled = true;

	}
	else
	{
		_editorView.renderTexture = nullptr;
		_editorView.enabled = false;
	}
	_kind = gui::WINDOWS::WIN_EDITOR_VIEW;

}

void EditorView::_DrawContentEditorCamera(Context &inContext)
{
	if (!_editorView.id.has_value()) return;

	fm::GameObject* camera = _editorScene->GetGameObjectByID(_editorView.id.value());
	std::shared_ptr<fm::Scene> currentScene = fm::Application::Get().GetScene(inContext.currentSceneName);
	if (camera != nullptr && currentScene != nullptr)
	{
		fm::Scene::MapOfGameObjects gos = currentScene->getAllGameObjects();
		{
			fm::CommandBuffer commandBuffer;
			commandBuffer.Clear(fm::BUFFER_BIT::COLOR_BUFFER_BIT | fm::BUFFER_BIT::DEPTH_BUFFER_BIT);
			camera->get<fmc::CCamera>()->AddCommandBuffer(fm::RENDER_QUEUE::FIRST_STATE, commandBuffer);
		}
		for (auto && o : gos)
		{
			fm::GameObject* go = o.second;
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


EditorView::~EditorView()
{
	delete _pickingSystem;
}

void EditorView::CustomDraw()
{

	if (_editorView.renderTexture != nullptr && _editorView.renderTexture->isCreated())
	{
		const fm::Texture texture = _editorView.renderTexture->GetColorBufferTexture(0);

		ImGui::SetCursorPos(_cursorPos);
		ImGui::Image((ImTextureID)texture.getID(), ImVec2(texture.getWidth(), texture.getHeight()));
		ImGuizmo::SetDrawlist();
		_scrollPos = fm::math::vec2(ImGui::GetScrollX(), ImGui::GetScrollY());

		_EditObject();
	}
    
}

void EditorView::BeforeWindowCreation()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
}
void EditorView::AfterWindowCreation()
{
	ImGui::PopStyleVar(1);
}

void EditorView::_EditObject()
{
	if (!_editorView.id.has_value()) return;

	if (!_gameObjectSelectedByPicking.has_value())
		return;

	
	fm::GameObject* camera = _editorScene->GetGameObjectByID(_editorView.id.value());

	ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
	if (_currentTransformContext == gui::TRANSFORM_CONTEXT::TRANSLATE)
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	else if (_currentTransformContext == gui::TRANSFORM_CONTEXT::ROTATE)
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	else if (_currentTransformContext == gui::TRANSFORM_CONTEXT::SCALE)
		mCurrentGizmoOperation = ImGuizmo::SCALE;

	fm::GameObject* go = fm::Application::Get().GetCurrentScene()->GetGameObjectByID(_gameObjectSelectedByPicking.value());
	if (go != nullptr)
	{
		fmc::CTransform* transform = go->get<fmc::CTransform>();

		ImGuiIO& io = ImGui::GetIO();
		//float scrollPosX = ImGui::GetScrollX();
		ImGuizmo::SetRect(_cursorPos.x + _startImagePos.x - ImGui::GetScrollX(),
			_cursorPos.y + _startImagePos.y - ImGui::GetScrollY(),
			_editorView.renderTexture->getWidth(),
			_editorView.renderTexture->getHeight());
		const fm::math::mat view = camera->get<fmc::CTransform>()->GetLocalMatrixModel();
		const fm::math::mat projecttion = camera->get<fmc::CCamera>()->GetProjectionMatrix();
		fm::math::mat model;

		fm::math::vec3 rotation = transform->GetRotation().GetEulerAngles();
		fm::math::vec3 position = transform->position;
		fm::math::vec3 scale = transform->scale;
		ImGuizmo::RecomposeMatrixFromComponents(&position[0], &rotation[0], &scale[0], &model[0][0]);

		ImGuizmo::Manipulate(&view[0][0], &projecttion[0][0], mCurrentGizmoOperation, mCurrentGizmoMode, &model[0][0], NULL, NULL);

		ImGuizmo::DecomposeMatrixToComponents(&model[0][0], &transform->position[0], &rotation[0], &transform->scale[0]);
		transform->SetRotation(fm::math::Quaternion::FromEulerAngles(rotation));
	}
		
}


void EditorView::_CallBackPickingSystem(ecs::id inID)
{
	_resultPicking = true;
	_gameObjectSelectedByPicking = inID;
}


void EditorView::SetPickingSystem(fms::PickingSystem *inPickingSystem)
{
	_pickingSystem = inPickingSystem;
	std::function<void(ecs::id inID)> f = std::bind(&gui::EditorView::_CallBackPickingSystem, this, std::placeholders::_1);
	_pickingSystem->SetCallback(std::move(f));
}


void EditorView::_Update(float dt, Context &inContext)
{
	_DrawContentEditorCamera(inContext);

	//assert(GImGui != nullptr && GImGui->CurrentWindow != nullptr);
	if (_editorView.enabled && _editorView.renderTexture != nullptr && _editorView.renderTexture->isCreated() && HasBeenDrawn())
	{
		fm::math::vec2 startCursorPos;

		const float rapport = (float)_editorView.renderTexture->getWidth() / (float)_editorView.renderTexture->getHeight();

		fm::math::vec2 start = GetPosition();
		fm::math::vec2 size = GetSize();
		fm::math::vec2 end(start.x + size.x, start.y + size.y);

		_startImagePos.x = start.x;
		_startImagePos.y = start.y;

		_cursorPos = fm::math::vec2(-(_editorView.renderTexture->getWidth() - size.x)*0.5f,
									-(_editorView.renderTexture->getHeight() - size.y)*0.5f);
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

	_currentTransformContext = inContext.currentTransformContext;
}



void EditorView::Resize() {}
void EditorView::Zoom() {}
