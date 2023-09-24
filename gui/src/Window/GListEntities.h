#pragma once
#include <memory>
#include "Window/GWindow.h"
#include <optional>
#include "Core/Observer.h"
#include <list>
#include "Core/LinkedTree.h"
#include "Entity.h"
namespace fm
{
	class GameObject;
	class Scene;
}

class LinkedTreeGO : public fm::LinkedTree<Entity::Id>
{
public:
	LinkedTreeGO() { _head = std::make_unique<Node<Entity::Id>>(Entity::INVALID); }

	void AddTo(std::unique_ptr<Node<Entity::Id>>& inNode, Entity::Id ID)
	{
		inNode->nodes.push_back(std::make_unique<Node<Entity::Id>>(ID));
	}

	void Sort(std::function<bool(std::unique_ptr<Node<Entity::Id>>&, std::unique_ptr<Node<Entity::Id>>&)>&& f);

	void Clear()
	{
		_head->nodes.clear();
	}

	std::unique_ptr<Node<Entity::Id>>& Find(Entity::Id ID, bool& outFound);

	Node<Entity::Id>* GetHead() { return _head.get(); }
private:

};


namespace gui
{
	class MainWindow;
	class GListEntities : public GWindow, public fm::Observer
	{
	public:
		GListEntities();
		~GListEntities();
		void CustomDraw();
		virtual void Notify(fm::Observable*, const fm::EventObserver& inEvent) override;
		void OnBeforeLoad(const std::string& inCurrentSceneName);
		void OnAfterLoad(const std::string& inCurrentSceneName);

		void PurgeTree();

	private:
		void _Update(float dt, Context &inContext);
		void _UpdateTree();


		void _IterateTree();
		void _IterateTree(LinkedTreeGO::Node<Entity::Id> *node);

		void _PurgeTree(LinkedTreeGO::Node<Entity::Id>* parent, const std::shared_ptr<fm::Scene> inScene);

		std::optional<Entity::Id>	_gameObjectSelected;
		std::string				_currentSceneName;
		bool					_isRenaming;
		char					_bufferName[128];
		bool					_goSelectedHasChanged = false;
		bool					_shouldUpdateListOrder = false;
		LinkedTreeGO			_order;


	};
}

