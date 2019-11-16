#include "GListEntities.h"
#include "GWindow.h"
#include "Core/SceneManager.h"
#include "Core/GameObject.h"
#include "Core/Scene.h"
#include "Components/CTransform.h"
using namespace gui;

GListEntities::GListEntities() : GWindow("List entities", true)
{
	_gameObjectSelected = nullptr;
	_enabled = true;
}


void GListEntities::Update(float dt, Context &inContext)
{

	if (_hasBeenSelected)
	{
		_hasBeenSelected = false;
		inContext.currentGameObjectSelected = _gameObjectSelected;
	}

	_gameObjectSelected = inContext.currentGameObjectSelected;
}

void GListEntities::CustomDraw()
{
	size_t entitySelected = -1;
	std::vector<fm::GameObject*> listEntities = fm::SceneManager::get().getCurrentScene()->getAllGameObjects();
	for (size_t i = 0; i < listEntities.size(); i++)
	{
		bool isSelected = (entitySelected == i)
			|| (_gameObjectSelected != nullptr && listEntities[i] != nullptr && (_gameObjectSelected->getID() == listEntities[i]->getID()));
		// Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
		ImGuiTreeNodeFlags node_flags = (isSelected ? ImGuiTreeNodeFlags_Selected : 0);
		if (isSelected)
		{
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.5f, 0.2f, 0.2f, 1.0f));
		}
		bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, listEntities[i]->name.c_str(), i);
		if (isSelected)
		{
			ImGui::PopStyleColor(1);
		}
		if (ImGui::IsItemClicked())
		{
			_gameObjectSelected = listEntities[i];
			_hasBeenSelected = true;
		}

		if (node_open)
		{
			ImGui::TreePop();
		}
	}

	if (ImGui::Button("Add Entity"))
	{
		_hasBeenSelected = true;
		_gameObjectSelected = fm::GameObjectHelper::create();
		_gameObjectSelected->addComponent<fmc::CTransform>(fm::math::Vector3f(0, 0, 0),
			fm::math::Vector3f(1, 1, 1),
			fm::math::vec3(0, 0, 0));
	}
}


GListEntities::~GListEntities()
{
}
