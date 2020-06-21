#pragma once
#include <memory>
#include "Window/GWindow.h"
#include <optional>
#include "Core/Observer.h"
#include <list>
#include "Core/LinkedTree.h"
namespace fm
{
	class GameObject;
	class Scene;
}

class LinkedTreeGO : public fm::LinkedTree<ecs::id>
{
public:
	LinkedTreeGO() { _head = std::make_unique<Node<ecs::id>>(-1); }

	void AddTo(std::unique_ptr<Node<ecs::id>>& inNode, ecs::id ID)
	{
		inNode->nodes.push_back(std::make_unique<Node<ecs::id>>(ID));
	}

	void Sort(std::function<bool(std::unique_ptr<Node<ecs::id>>&, std::unique_ptr<Node<ecs::id>>&)>&& f);

	void Clear()
	{
		_head->nodes.clear();
	}

	std::unique_ptr<Node<ecs::id>>& Find(ecs::id ID, bool& outFound);

	Node<ecs::id>* GetHead() { return _head.get(); }
private:

};


namespace gui
{
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
		void _IterateTree(LinkedTreeGO::Node<ecs::id> *node);

		void _PurgeTree(LinkedTreeGO::Node<ecs::id>* parent, const std::shared_ptr<fm::Scene> inScene);

		std::optional<ecs::id>	_gameObjectSelected;
		std::string				_currentSceneName;
		bool					_isRenaming;
		char					_bufferName[128];
		bool					_goSelectedHasChanged = false;
		bool					_shouldUpdateListOrder = false;
		LinkedTreeGO			_order;


	};
}

