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
#include "Rendering/commandBuffer.hpp"
#include "Rendering/OpenGL/OGLTexture.hpp"
#include "Rendering/OpenGL/OGLFrameBuffer.hpp"
#include "Rendering/RenderTexture.hpp"
#include "Core/GameObject.h"
using namespace gui;
GEditorView::GEditorView(std::shared_ptr<fm::GameObject> inCamera, std::shared_ptr<fm::Scene> inScene) :
	GWindow("Editor View", true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)
{
	_editorScene = inScene;
	_enabled = true;

	_pickingSystem = nullptr;
	_resultPicking = false;

	if (inCamera != nullptr)
	{
		_editorView.id = inCamera->GetID();
		fmc::CCamera &camera = inCamera->get<fmc::CCamera>();
		_editorView.renderTexture = std::make_shared<fm::OGLFrameBuffer>(*fm::CreateRenderTexture(camera.GetWidth(), camera.GetHeight(), 0));
		camera.SetTarget(_editorView.renderTexture);
	}

	_kind = gui::WINDOWS::WIN_EDITOR_VIEW;

}

void GEditorView::_MoveCamera(float dt, const Context& inContext)
{
	if (!_editorView.id.has_value()) return;

	if (auto&& editorScene = _editorScene.lock())
	{
		std::shared_ptr<fm::GameObject> camera = editorScene->GetGameObjectByID(_editorView.id.value());
		std::shared_ptr<fm::Scene> currentScene = fm::Application::Get().GetScene(inContext.currentSceneName);
		if (camera != nullptr && camera->IsActive() && currentScene != nullptr)
		{
			fmc::CTransform& transform = camera->get<fmc::CTransform>();

			if (ImGui::IsMouseDown(1))
			{
				const fm::math::vec2 mousePos = ImGui::GetMousePos();
				fm::math::vec2 direction = _mousePos - mousePos;
				if (std::abs(direction.x) > std::abs(direction.y))
					direction.y = 0;
				else
					direction.x = 0;

				const fm::math::Quaternion rotate = fm::math::Quaternion::FromEulerAngles(fm::math::vec3(direction.y, direction.x, 0)*dt*5.0f);
				
				transform.SetRotation(rotate* transform.GetRotation());
			}

			if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
			{
				fm::math::vec3 position = transform.GetPosition();
				const fm::math::vec2 mousePos = ImGui::GetMousePos();
				const fm::math::vec2 direction = _mousePos - mousePos;
				const fm::math::mat rotation = transform.GetRotation().GetRotationMatrix();
				position += rotation *(fm::math::vec3(direction.x, -direction.y, 0)) * dt * 1.0f;
				transform.SetPosition(position);
			}

			if (ImGui::IsKeyDown(ImGuiKey_MouseWheelY))
			{
				fm::math::vec3 position = transform.GetPosition();
				float wheelPos = ImGui::GetIO().MouseWheel;
				const fm::math::mat rotation = transform.GetRotation().GetRotationMatrix();
				position += rotation * (fm::math::vec3(0, 0, wheelPos)) * dt * 20.0f;
				transform.SetPosition(position);
			}
		}
	}
	_mousePos = ImGui::GetMousePos();
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
				camera->get<fmc::CCamera>().AddCommandBuffer(fm::RENDER_QUEUE_FIRST_STATE, commandBuffer);
			}

			{
				fm::CommandBuffer commandBuffer;
				commandBuffer.Enable(fm::RENDERING_TYPE::DEPTH_TEST);
				camera->get<fmc::CCamera>().AddCommandBuffer(fm::RENDER_QUEUE_FIRST_STATE, commandBuffer);
			}

			std::vector<std::shared_ptr<fm::Model>> models;
			std::vector<std::shared_ptr<fm::Material>> materials;
			std::vector<fm::Transform> transforms;


			for (auto&& o : gos)
			{
				std::shared_ptr<fm::GameObject> go = o.second;
				if (!go->IsActive())
					continue;
				if (go->has<fmc::CTransform>() && go->has<fmc::CMesh>() && go->has<fmc::CMaterial>())
				{
					fmc::CMesh& mesh = go->get<fmc::CMesh>();
					auto mat = go->get<fmc::CMaterial>();
					materials.emplace_back(mat.GetMainMaterial());
					models.emplace_back(mesh.GetModel());
					transforms.emplace_back(go->get<fmc::CTransform>().GetTransform());
				}
			}

			fm::CommandBuffer commandBuffer;
			commandBuffer.DrawInstancedMesh(models, transforms, materials);
			camera->get<fmc::CCamera>().AddCommandBuffer(fm::RENDER_QUEUE_BEFORE_RENDERING_FILL_QUEUE, commandBuffer);

		}
	}
}


GEditorView::~GEditorView()
{
}

void GEditorView::CustomDraw()
{

	if (auto & renderTexture = _editorView.renderTexture)
	{
		if (renderTexture->IsCreated())
		{
			const std::shared_ptr<fm::OGLTexture> texture = std::dynamic_pointer_cast<fm::OGLFrameBuffer>(renderTexture)->GetColorBufferTexture(0);

			ImGui::SetCursorPos(_cursorPos);
			ImGui::Image(ImTextureID((intptr_t)texture->getID()), ImVec2(texture->getWidth(), texture->getHeight()));
			ImGuizmo::SetDrawlist();
			_scrollPos = fm::math::vec2(ImGui::GetScrollX(), ImGui::GetScrollY());

			_EditObject();

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FileNavigator"))
				{
					fm::math::vec2 mousePos = ImGui::GetMousePos();
					fm::math::vec2 start = GetPosition();
					fm::math::vec2 startCursorPos;
					startCursorPos = _cursorPos;
					startCursorPos.y += start.y;
					startCursorPos.x += start.x;

					fm::math::vec2 mPos(mousePos);
					mPos.x -= startCursorPos.x - _scrollPos.x;
					mPos.y -= startCursorPos.y - _scrollPos.y;

					char* payload_n = (char*)payload->Data;
					std::string data(payload_n);
					fm::FilePath path = fm::FilePath(data);
					AddEvent([this, mPos, path](gui::GWindow*, std::optional<gui::Context> inContext)
					{
							_pickingSystem->PickGameObject(inContext->currentSceneName, _editorView.id.value(), mPos,
							[this, path](fm::GameObjectID_t id) {
									_resultPicking = true;
									_gameObjectSelectedByPicking = id;
									if (path.GetExtension() == ".material")
									{
										std::shared_ptr<fm::GameObject> go = fm::Application::Get().GetCurrentScene()->GetGameObjectByID(id);
										if (go != nullptr)
										{
											auto cmaterial = go->get<fmc::CMaterial>();
											cmaterial.SetMainMaterial(path);
										}
									}

							});

					});

				}

				ImGui::EndDragDropTarget();
			}
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
			if (auto&& renderTexture = _editorView.renderTexture)
			{
				fmc::CTransform& transform = go->get<fmc::CTransform>();
				//delete transform;
				ImGuiIO& io = ImGui::GetIO();
				//float scrollPosX = ImGui::GetScrollX();
				ImGuizmo::SetRect(_cursorPos.x + _startImagePos.x - ImGui::GetScrollX(),
					_cursorPos.y + _startImagePos.y - ImGui::GetScrollY(),
					renderTexture->GetWidth(),
					renderTexture->GetHeight());
				const fm::math::mat view = camera->get<fmc::CCamera>().GetViewMatrix();
				const fm::math::mat projection = camera->get<fmc::CCamera>().GetProjectionMatrix();
				fm::math::mat model;

				fm::math::vec3 rotation = transform.GetRotation().GetEulerAngles();
				fm::math::vec3 position = transform.GetPosition();
				fm::math::vec3 scale = transform.GetScale();
				ImGuizmo::RecomposeMatrixFromComponents(&position[0], &rotation[0], &scale[0], &model[0][0]);

				ImGuizmo::Manipulate(&view[0][0], &projection[0][0], mCurrentGizmoOperation, mCurrentGizmoMode, &model[0][0], NULL, NULL);

				ImGuizmo::DecomposeMatrixToComponents(&model[0][0], &position[0], &rotation[0], &scale[0]);
				transform.SetPosition(position);
				transform.SetScale(scale);
				transform.SetRotation(fm::math::Quaternion::FromEulerAngles(rotation));
			}
		}
	}
		
}


void GEditorView::_CallBackPickingSystem(fm::GameObjectID_t inID)
{
	_resultPicking = true;
	_gameObjectSelectedByPicking = inID;
}


void GEditorView::SetPickingSystem(std::unique_ptr<fms::PickingSystem> inPickingSystem)
{
	_pickingSystem = std::move(inPickingSystem);
	std::function<void(fm::GameObjectID_t)> f = std::bind(&gui::GEditorView::_CallBackPickingSystem, this, std::placeholders::_1);
	_pickingSystem->SetCallback(std::move(f));
}


void GEditorView::_Update(float dt, Context &inContext)
{
	_DrawContentEditorCamera(inContext);

	//assert(GImGui != nullptr && GImGui->CurrentWindow != nullptr);
	if (auto& renderTexture = _editorView.renderTexture)
	{
		_MoveCamera(dt, inContext);
		if (renderTexture->IsCreated() && HasBeenDrawn())
		{

			const float rapport = (float)renderTexture->GetWidth() / (float)renderTexture->GetHeight();

			fm::math::vec2 start = GetPosition();
			fm::math::vec2 size = GetSize();
			fm::math::vec2 end(start.x + size.x, start.y + size.y);

			_startImagePos.x = start.x;
			_startImagePos.y = start.y;

			_cursorPos = fm::math::vec2(-(renderTexture->GetWidth() - size.x) * 0.5f,
				-(renderTexture->GetHeight() - size.y) * 0.5f);
			fm::math::vec2 startCursorPos;
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
				inContext.currentWindowToDisplay = gui::WINDOWS::WIN_LIST_ENTITIES;
				_resultPicking = false;
			}
			_gameObjectSelectedByPicking = inContext.currentGameObjectSelected;

		}
	}

	_currentTransformContext = inContext.currentTransformContext;
}



void GEditorView::Resize() {}
void GEditorView::Zoom() {}
