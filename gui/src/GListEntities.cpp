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


void LinkedTreeGO::Sort(std::function<bool(std::unique_ptr<Node<ecs::id>>&, std::unique_ptr<Node<ecs::id>>&b)> && f)
{
	std::stack<Node<ecs::id>*> stack;
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


std::unique_ptr<LinkedTreeGO::Node<ecs::id>>& LinkedTreeGO::Find(ecs::id ID, bool& outFound)
{
	if (ID == -1)
	{
		outFound = true;
		return _head;
	}
	else
	{
		std::stack<Node<ecs::id>*> stack;
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


void GListEntities::_PurgeTree(LinkedTreeGO::Node<ecs::id>* parent, const std::shared_ptr<fm::Scene> inScene)
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
	fm::Scene::MapOfGameObjects listEntities = currentScene->getAllGameObjects();

	PurgeTree();

	for (auto&& e : listEntities)
	{
		fmc::CTransform* tr = EntityManager::get().getEntity(e.first)->get<fmc::CTransform>();
		std::stack<ecs::id> fathers;
		while (tr->HasFather())
		{
			fathers.push(tr->GetFatherID());
			tr = tr->GetFather();
		}
		fathers.push(e.first);

		while (!fathers.empty())
		{
			bool found = false;
			ecs::id fatherID = fathers.top();
			fathers.pop();
			auto& node = _order.Find(fatherID, found);
			if (!found)
			{
				_order.AddTo(node, e.first);
			}
			
		}
	}

	_order.Sort([&currentScene](std::unique_ptr<LinkedTreeGO::Node<ecs::id>>& a, std::unique_ptr<LinkedTreeGO::Node<ecs::id>>& b)
	{
			std::shared_ptr<fm::GameObject> oa = currentScene->GetGameObjectByID(a->value);
			std::shared_ptr<fm::GameObject> ob = currentScene->GetGameObjectByID(b->value);
			return oa->GetOrder() < ob->GetOrder();

	});
	
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
}


void GListEntities::_Update(float dt, Context &inContext)
{
	if (!inContext.currentSceneName.empty() && _currentSceneName != inContext.currentSceneName)
	{
		std::shared_ptr<fm::Scene> oldScene = fm::Application::Get().GetScene(_currentSceneName);
		if(oldScene != nullptr)
			oldScene->Unsubscribe(this);
		_currentSceneName = inContext.currentSceneName;
		fm::Application::Get().GetScene(_currentSceneName)->Subscribe(this);
		_order.Clear();
		_UpdateTree();
	}

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
	}
}

void GListEntities::_IterateTree(LinkedTreeGO::Node<ecs::id>* node)
{
	size_t i = 0;
	for (auto&& n : node->nodes)
	{
		bool isSelected = _gameObjectSelected.has_value() && (_gameObjectSelected.value() == n->value);
		std::shared_ptr<fm::GameObject> go = fm::Application::Get().GetScene(_currentSceneName)->GetGameObjectByID(n->value);
		if (go != nullptr)
		{
			if (!go->IsActive())
			{
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}

			ImGuiTreeNodeFlags node_flags = (isSelected ? ImGuiTreeNodeFlags_Selected : 0);
			if (isSelected)
			{
				ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.5f, 0.2f, 0.2f, 1.0f));
			}
			float oldPos = ImGui::GetCursorPosX();
			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, go->GetName().c_str(), i);
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


			if (node_open)
			{
				if (!n->nodes.empty())
				{
					_IterateTree(n.get());
				}
				ImGui::TreePop();
			}

			if (!go->IsActive())
			{
				ImGui::PopStyleVar();
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
		fm::Scene::MapOfGameObjects listEntities = currentScene->getAllGameObjects();
		size_t i = 0;

		_IterateTree();

		if (ImGui::IsMouseClicked(1))
		{
			ImGui::OpenPopup("popup from button");
		}

		std::shared_ptr<fm::GameObject> goSelected = _gameObjectSelected.has_value() ? listEntities[_gameObjectSelected.value()] : nullptr;

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
					AddEvent([id, scene](gui::GWindow* window) {
						std::shared_ptr<fm::Scene> currentScene = fm::Application::Get().GetScene(scene);
						currentScene->DeleteGameObjectByID(id);

						});
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
