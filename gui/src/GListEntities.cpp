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
	_gameObjectSelected = nullptr;
	_enabled = true;
	_kind = gui::WINDOWS::WIN_LIST_ENTITIES;
	_isRenaming = false;
	memset(_bufferName, '\0', 128);
}


void GListEntities::_Update(float dt, Context &inContext)
{
	_currentSceneName = inContext.currentSceneName;
	if (_hasBeenSelected)
	{
		_hasBeenSelected = false;
		inContext.currentGameObjectSelected = _gameObjectSelected;
	}

	_gameObjectSelected = inContext.currentGameObjectSelected;
}



void GListEntities::CustomDraw()
{
	std::shared_ptr<fm::Scene> currentScene = fm::Application::Get().GetScene(_currentSceneName);
	size_t itemHovered = -1;
	if (currentScene != nullptr)
	{
		std::vector<fm::GameObject*> listEntities = currentScene->getAllGameObjects();

		for (size_t i = 0; i < listEntities.size(); i++)
		{
			bool isSelected = _itemSelected == i;

			if (_isRenaming && isSelected)
			{
				fm::GameObject* go = listEntities[i];
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
				bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, listEntities[i]->GetName().c_str(), i);
				if (isSelected)
				{
					ImGui::PopStyleColor(1);
				}

				if (ImGui::IsItemClicked())
				{
					_itemSelected = i;
				}


				if (node_open)
				{
					ImGui::TreePop();
				}
			}
			
		}





		if (ImGui::IsMouseClicked(1))
		{
			ImGui::OpenPopup("popup from button");
		}

		if (ImGui::BeginPopup("popup from button") && _itemSelected != -1)
		{
			if (ImGui::MenuItem("Rename"))
			{
				_isRenaming = true;
			}

			ImGui::EndPopup();
		}



		if (ImGui::Button("Add Entity"))
		{
			_hasBeenSelected = true;
			_gameObjectSelected = fm::GameObjectHelper::create(currentScene, true);
			
		}
	}
}


GListEntities::~GListEntities()
{
}
