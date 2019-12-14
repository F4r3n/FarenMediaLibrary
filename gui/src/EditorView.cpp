#include "EditorView.h"
#include "Components/CCamera.h"
#include "Core/Scene.h"
#include "Core/SceneManager.h"
#include "PickingSystem.h"
#include "ImGuizmo/ImGuizmo.h"
#include "Input/InputManager.h"
#include <imgui/imgui_internal.h>
#include "Core/application.h"

using namespace gui;
EditorView::EditorView(fm::GameObject* inCamera, std::shared_ptr<fm::Scene> inScene) : GWindow("Editor View", true, ImGuiWindowFlags_HorizontalScrollbar
												)
{
	_editorScene = inScene;
	_enabled = true;
	_gameObjectSelectedByPicking = nullptr;

	_pickingSystem = nullptr;
	_resultPicking = false;

	if (inCamera != nullptr)
	{
		_editorView.id = inScene->GetID(inCamera->getID());
		fmc::CCamera *camera = inCamera->get<fmc::CCamera>();
		_editorView.renderTexture = camera->SetTarget();
		_editorView.enabled = true;

	}
	else
	{
		_editorView.id = 0;
		_editorView.renderTexture = nullptr;
		_editorView.enabled = false;
	}
	_kind = gui::WINDOWS::WIN_EDITOR_VIEW;

}

void EditorView::_DrawContentEditorCamera(Context &inContext)
{
	fm::GameObject* camera = _editorScene->GetGameObject(_editorView.id);
	std::shared_ptr<fm::Scene> currentScene = fm::Application::Get().GetScene(inContext.currentSceneName);
	if (camera != nullptr && currentScene != nullptr)
	{
		std::vector<fm::GameObject*> &&gos = currentScene->getAllGameObjects();
		for (auto && go : gos)
		{
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

		ImGui::SetCursorPos(ImVec2(_cursorPos.x, _cursorPos.y));
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

	fm::GameObject* camera = _editorScene->GetGameObject(_editorView.id);
	if (_gameObjectSelectedByPicking == nullptr || !_gameObjectSelectedByPicking->IsActive())
		return;

	ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
	if (_currentTransformContext == gui::TRANSFORM_CONTEXT::TRANSLATE)
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	else if (_currentTransformContext == gui::TRANSFORM_CONTEXT::ROTATE)
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	else if (_currentTransformContext == gui::TRANSFORM_CONTEXT::SCALE)
		mCurrentGizmoOperation = ImGuizmo::SCALE;

	fmc::CTransform *transform = _gameObjectSelectedByPicking->get<fmc::CTransform>();

	
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


void EditorView::_CallBackPickingSystem(fm::GameObject* inGameObject)
{
	_resultPicking = true;
	_gameObjectSelectedByPicking = inGameObject;
}


void EditorView::SetPickingSystem(fms::PickingSystem *inPickingSystem)
{
	_pickingSystem = inPickingSystem;
	std::function<void(fm::GameObject*)> f = std::bind(&gui::EditorView::_CallBackPickingSystem, this, std::placeholders::_1);
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
			ImVec2 mousePos = ImGui::GetMousePos();

			fm::Rectf rect;
			rect.w = size.x;
			rect.h = size.y;
			rect.x = start.x;
			rect.y = start.y;


			if (rect.contains(mousePos.x, mousePos.y))
			{
				fm::math::vec2 mPos(mousePos.x, mousePos.y);
				mPos.x -= startCursorPos.x - _scrollPos.x;
				mPos.y -= startCursorPos.y - _scrollPos.y;

				_pickingSystem->PickGameObject(inContext.currentSceneName, _editorView.id, mPos);
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
