#include "GListEntities.h"
#include "GWindow.h"
#include "Core/SceneManager.h"
#include "Core/GameObject.h"
#include "Core/Scene.h"
#include "Components/CTransform.h"
using namespace gui;

GListEntities::GListEntities() : GWindow("List entities", true)
{
	_timerListEntityUpdate = 1;
	_entitiesSelected = 0;
	_gameObjectSelected = nullptr;
	_enabled = true;
}


void GListEntities::_RefreshAllEntities()
{
	_namesEntities.clear();
	std::vector<fm::GameObject*> gos = fm::SceneManager::get().getCurrentScene()->getAllGameObjects();
	for (auto e : gos) 
	{
		_namesEntities.push_back(e->name.c_str());
	}
}



void GListEntities::Update(float dt)
{
	if (_timerListEntityUpdate > 1)
	{
		_RefreshAllEntities();
		_timerListEntityUpdate = 0;
	}
	else
	{
		_timerListEntityUpdate += dt;
	}
}

void GListEntities::CustomDraw()
{
	_gameObjectSelected = nullptr;
	for (size_t i = 0; i < _namesEntities.size(); i++)
	{
		// Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
		ImGuiTreeNodeFlags node_flags = ((_entitiesSelected == i) ? ImGuiTreeNodeFlags_Selected : 0);

		bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, _namesEntities[i], i);

		if (ImGui::IsItemClicked())
		{
			_entitiesSelected = i;
			_gameObjectSelected = fm::SceneManager::get().getCurrentScene()->getAllGameObjects()[i];
		}

		if (node_open)
		{
			ImGui::TreePop();
		}
	}

	if (ImGui::Button("Add Entity"))
	{
		_gameObjectSelected = fm::GameObjectHelper::create();
		_gameObjectSelected->addComponent<fmc::CTransform>(fm::math::Vector3f(0, 0, 0),
			fm::math::Vector3f(1, 1, 1),
			fm::math::vec3(0, 0, 0));
	}
}


bool GListEntities::HasGameObjectSelected()
{
	return _gameObjectSelected != nullptr;
}

fm::GameObject* GListEntities::GetGameObjectSelected()
{
	return _gameObjectSelected;
}

GListEntities::~GListEntities()
{
}
