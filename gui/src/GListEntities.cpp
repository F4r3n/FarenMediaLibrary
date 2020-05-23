#include "GListEntities.h"
#include "GWindow.h"
#include "Core/SceneManager.h"
#include "Core/GameObject.h"
#include "Core/Scene.h"
#include "Components/CTransform.h"
#include "Core/application.h"
using namespace gui;

GListEntities::GListEntities() : GWindow("List entities", true)
{
	_enabled = true;
	_kind = gui::WINDOWS::WIN_LIST_ENTITIES;
	_isRenaming = false;
	memset(_bufferName, '\0', 128);
}


void GListEntities::_Update(float dt, Context &inContext)
{
	_currentSceneName = inContext.currentSceneName;
	if (_goSelectedHasChanged)
	{
		inContext.currentGameObjectSelected = _gameObjectSelected;
		_gameObjectSelected.reset();
		_goSelectedHasChanged = false;
	}

	_gameObjectSelected = inContext.currentGameObjectSelected;
}



void GListEntities::CustomDraw()
{
	std::shared_ptr<fm::Scene> currentScene = fm::Application::Get().GetScene(_currentSceneName);
	if (currentScene != nullptr)
	{
		std::map<ecs::id, fm::GameObject*> listEntities = currentScene->getAllGameObjects();
		size_t i = 0;
		for (auto && o : listEntities)
		{
			bool isSelected = _itemSelected == o.first;

			fm::GameObject* go = o.second;

			if (!go->IsActive())
			{
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}

			if (_isRenaming && isSelected)
			{
				memcpy(_bufferName, go->GetName().c_str(), std::min((size_t)127, go->GetName().size()));
				if (ImGui::InputText("##", _bufferName, 128, ImGuiInputTextFlags_EnterReturnsTrue) && HasFocus())
				{
					std::string newName(_bufferName);
					go->SetName(_bufferName);
					_isRenaming = false;
				}
				
			}
			else
			{
				// Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
				ImGuiTreeNodeFlags node_flags = (isSelected ? ImGuiTreeNodeFlags_Selected : 0);
				if (isSelected)
				{
					ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.5f, 0.2f, 0.2f, 1.0f));
				}
				bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, go->GetName().c_str(), i);
				if (isSelected)
				{
					ImGui::PopStyleColor(1);
				}

				if (ImGui::IsItemClicked())
				{
					_itemSelected = o.first;
				}


				if (node_open)
				{
					ImGui::TreePop();
				}
			}

			if (!go->IsActive())
			{
				ImGui::PopStyleVar();
			}
			i++;
		}



		if (ImGui::IsMouseClicked(1))
		{
			ImGui::OpenPopup("popup from button");
		}

		fm::GameObject* goSelected = _itemSelected != -1 ? listEntities[_itemSelected] : nullptr;

		if (goSelected != nullptr)
		{
			if (ImGui::BeginPopup("popup from button"))
			{
				if (ImGui::MenuItem("Rename"))
				{
					_isRenaming = true;
				}
				else if (ImGui::MenuItem("Delete"))
				{
					size_t id = goSelected->getID();
					std::string scene(_currentSceneName);
					AddEvent([id, scene](gui::GWindow*) {
						std::shared_ptr<fm::Scene> currentScene = fm::Application::Get().GetScene(scene);
						currentScene->DeleteGameObjectByID(id);
						});
					_itemSelected = -1;
					_gameObjectSelected.reset();
					_goSelectedHasChanged = true;
				}
				else if (goSelected->IsActive() && ImGui::MenuItem("Disable"))
				{
					goSelected->SetStatus(false);
				}
				else if (!goSelected->IsActive() && ImGui::MenuItem("Activate"))
				{
					goSelected->SetStatus(true);
				}

				ImGui::EndPopup();
			}
		}



		if (ImGui::Button("Add Entity"))
		{
			_gameObjectSelected = fm::GameObjectHelper::create(currentScene, true)->getID();
		}
	}
}


GListEntities::~GListEntities()
{
}
