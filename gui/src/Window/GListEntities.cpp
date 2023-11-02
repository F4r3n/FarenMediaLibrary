#include "GListEntities.h"
#include "GWindow.h"
#include "Core/SceneManager.h"
#include "Core/GameObject.h"
#include "Core/Scene.h"
#include "Components/CTransform.h"
#include "Core/application.h"
#include <stack>
#include <algorithm>
using namespace gui;


void LinkedTreeGO::Sort(std::function<bool(std::unique_ptr<Node<Entity::Id>>&, std::unique_ptr<Node<Entity::Id>>&b)> && f)
{
	std::stack<Node<Entity::Id>*> stack;
	stack.push(_head.get());

	while (!stack.empty())
	{
		auto& list = stack.top();
		for (auto&& n : list->nodes)
		{
			if (!n->nodes.empty())
			{
				stack.push(n.get());
			}
		}
		
		list->nodes.sort(f);
		stack.pop();
	}
}


std::unique_ptr<LinkedTreeGO::Node<Entity::Id>>& LinkedTreeGO::Find(Entity::Id ID, bool& outFound)
{
	if (ID == Entity::INVALID)
	{
		outFound = true;
		return _head;
	}
	else
	{
		std::stack<Node<Entity::Id>*> stack;
		stack.push(_head.get());

		while (!stack.empty())
		{
			auto& list = stack.top();
			for (auto&& n : list->nodes)
			{
				if (n->value == ID)
				{
					outFound = true;
					return n;
				}
				else if (!n->nodes.empty())
				{
					stack.push(n.get());
				}
			}
			stack.pop();
		}
	}
	outFound = false;
	return _head;
}


GListEntities::GListEntities() : GWindow("List entities", true)
{
	_enabled = true;
	_kind = gui::WINDOWS::WIN_LIST_ENTITIES;
	_isRenaming = false;
	memset(_bufferName, '\0', 128);

}


void GListEntities::PurgeTree()
{
	std::shared_ptr<fm::Scene> currentScene = fm::Application::Get().GetScene(_currentSceneName);
	_PurgeTree(_order.GetHead(), currentScene);
}


void GListEntities::_PurgeTree(LinkedTreeGO::Node<Entity::Id>* parent, const std::shared_ptr<fm::Scene> inScene)
{
	auto it = parent->nodes.begin();
	while(it != parent->nodes.end())
	{
		if (inScene->GetGameObjectByID((*it)->value) == nullptr)
		{
			parent->nodes.erase(it++);
		}
		else
		{
			if (!(*it)->nodes.empty())
			{
				_PurgeTree((*it).get(), inScene);
			}
			++it;
		}
	}
}


void GListEntities::_UpdateTree()
{
	std::shared_ptr<fm::Scene> currentScene = fm::Application::Get().GetScene(_currentSceneName);

	if (currentScene != nullptr)
	{
		fm::Scene::MapOfGameObjects listEntities = currentScene->GetAllGameObjects();

		PurgeTree();

		for (auto&& e : listEntities)
		{
			fmc::CTransform* tr = EntityManager::get().GetEntity(e.first).get<fmc::CTransform>();
			std::stack<Entity::Id> fathers;
			while (tr->HasFather())
			{
				fathers.push(tr->GetFatherID());
				tr = tr->GetFather();
			}
			fathers.push(e.first);

			while (!fathers.empty())
			{
				bool found = false;
				Entity::Id fatherID = fathers.top();
				fathers.pop();
				auto& node = _order.Find(fatherID, found);
				if (!found)
				{
					_order.AddTo(node, e.first);
				}

			}
		}

		_order.Sort([&currentScene](std::unique_ptr<LinkedTreeGO::Node<Entity::Id>>& a, std::unique_ptr<LinkedTreeGO::Node<Entity::Id>>& b)
			{
				std::shared_ptr<fm::GameObject> oa = currentScene->GetGameObjectByID(a->value);
				std::shared_ptr<fm::GameObject> ob = currentScene->GetGameObjectByID(b->value);
				return oa->GetOrder() < ob->GetOrder();

			});
	}
	
}

void GListEntities::OnBeforeLoad(const std::string& inCurrentSceneName)
{
	std::shared_ptr<fm::Scene> oldScene = fm::Application::Get().GetScene(inCurrentSceneName);
	if (oldScene != nullptr)
		oldScene->Unsubscribe(this);
	_order.Clear();
	_currentSceneName = "";
}

void GListEntities::OnAfterLoad(const std::string& inCurrentSceneName)
{
	std::shared_ptr<fm::Scene> newScene = fm::Application::Get().GetScene(inCurrentSceneName);
	if (newScene != nullptr)
		newScene->Subscribe(this);

	_currentSceneName = inCurrentSceneName;

	_order.Clear();
	_shouldUpdateListOrder = true;
}


void GListEntities::_Update(float dt, Context &inContext)
{

	if (_goSelectedHasChanged)
	{
		inContext.currentGameObjectSelected = _gameObjectSelected;
		_gameObjectSelected.reset();
		_goSelectedHasChanged = false;
	}

	_gameObjectSelected = inContext.currentGameObjectSelected;


	if (_shouldUpdateListOrder)
	{
		_UpdateTree();
		_shouldUpdateListOrder = false;
	}
}

void GListEntities::_IterateTree(LinkedTreeGO::Node<Entity::Id>* node)
{
	size_t i = 0;
	for (auto&& n : node->nodes)
	{
		bool isSelected = _gameObjectSelected.has_value() && (_gameObjectSelected.value() == n->value);
		std::shared_ptr<fm::GameObject> go = fm::Application::Get().GetScene(_currentSceneName)->GetGameObjectByID(n->value);
		if (go != nullptr)
		{


			ImGuiTreeNodeFlags node_flags = (isSelected ? ImGuiTreeNodeFlags_Selected : 0);
			if (isSelected)
			{
				ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.5f, 0.2f, 0.2f, 1.0f));
			}
			float oldPos = ImGui::GetCursorPosX();
			std::string nameID = "ID" + std::to_string(i);
			if (!go->IsActive())
			{
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}

			bool node_open = ImGui::TreeNodeEx(nameID.c_str(), node_flags, go->GetName().c_str(), i);
			if (!go->IsActive())
			{
				ImGui::PopStyleVar();
			}
			if (_isRenaming && isSelected)
			{

				memcpy(_bufferName, go->GetName().c_str(), std::min((size_t)127, go->GetName().size()));
				ImGui::SameLine();
				ImGui::SetCursorPosX(oldPos);
				if (ImGui::InputText("##", _bufferName, 128, ImGuiInputTextFlags_EnterReturnsTrue) && HasFocus())
				{
					std::string newName(_bufferName);
					go->SetName(_bufferName);
					_isRenaming = false;
				}

			}


			if (isSelected)
			{
				ImGui::PopStyleColor(1);
			}

			if (ImGui::IsItemClicked())
			{
				_gameObjectSelected = n->value;
				_goSelectedHasChanged = true;
			}
			std::string namePopUp = "Popup" + nameID;

			if (ImGui::IsItemHovered() && ImGui::IsItemClicked(1))
			{
				ImGui::OpenPopup(namePopUp.c_str());
			}


			if (ImGui::BeginPopup(namePopUp.c_str()))
			{
				if (ImGui::MenuItem("Rename"))
				{
					_isRenaming = true;
				}
				else if (ImGui::MenuItem("Delete"))
				{
					Entity::Id id = go->getID();
					std::string scene(_currentSceneName);
					AddEvent([id, scene](gui::GWindow* window, std::optional<gui::Context> Context) {
						std::shared_ptr<fm::Scene> currentScene = fm::Application::Get().GetScene(scene);
						currentScene->DeleteGameObjectByID(id);

						});
					_gameObjectSelected.reset();
					_goSelectedHasChanged = true;
				}
				else if (go->IsActive() && ImGui::MenuItem("Disable"))
				{
					go->SetStatus(false);
				}
				else if (!go->IsActive() && ImGui::MenuItem("Activate"))
				{
					go->SetStatus(true);
				}

				ImGui::EndPopup();
			}
			
			


			if (node_open)
			{
				if (!n->nodes.empty())
				{
					_IterateTree(n.get());
				}
				ImGui::TreePop();
			}




			i++;
		}
	}
}


void GListEntities::_IterateTree()
{
	auto head = _order.GetHead();

	_IterateTree(head);

}


void GListEntities::CustomDraw()
{
	std::shared_ptr<fm::Scene> currentScene = fm::Application::Get().GetScene(_currentSceneName);
	if (currentScene != nullptr)
	{
		fm::Scene::MapOfGameObjects listEntities = currentScene->GetAllGameObjects();
		size_t i = 0;

		_IterateTree();
		
		std::shared_ptr<fm::GameObject> goSelected = _gameObjectSelected.has_value() ? listEntities[_gameObjectSelected.value()] : nullptr;

		if (goSelected != nullptr)
		{

		}


		if (ImGui::Button("Add Entity"))
		{
			_gameObjectSelected = currentScene->CreateGameObject(true)->getID();
		}
	}
}

void GListEntities::Notify(fm::Observable*, const fm::EventObserver& inEvent)
{
	_shouldUpdateListOrder = true;
}


GListEntities::~GListEntities()
{
}
