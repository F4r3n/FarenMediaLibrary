#pragma once
#include <memory>
#include "Window/GWindow.h"
#include <optional>
#include "Core/Observer.h"
#include <list>
#include "Core/LinkedTree.h"
#include "Core/GameObjectType.hpp"

namespace fm
{
	class GameObject;
	class Scene;
}

class LinkedTreeGO : public fm::LinkedTree<fm::GameObjectID_t>
{
public:
	LinkedTreeGO() { _head = std::make_unique<Node<fm::GameObjectID_t>>(0); }

	void AddTo(std::unique_ptr<Node<fm::GameObjectID_t>>& inNode, fm::GameObjectID_t ID)
	{
		inNode->nodes.push_back(std::make_unique<Node<fm::GameObjectID_t>>(ID));
	}

	void Sort(std::function<bool(std::unique_ptr<Node<fm::GameObjectID_t>>&, std::unique_ptr<Node<fm::GameObjectID_t>>&)>&& f);

	void Clear()
	{
		_head->nodes.clear();
	}

	std::unique_ptr<Node<fm::GameObjectID_t>>& Find(fm::GameObjectID_t ID, bool& outFound);

	Node<fm::GameObjectID_t>* GetHead() { return _head.get(); }
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
		virtual void CustomDraw() override;
		virtual void Notify(fm::Observable*, const fm::EventObserver& inEvent) override;
		void OnBeforeLoad(const std::string& inCurrentSceneName);
		void OnAfterLoad(const std::string& inCurrentSceneName);

		void PurgeTree();

	private:
		virtual void _Update(float dt, Context &inContext) override;
		void _UpdateTree();


		void _IterateTree();
		void _IterateTree(LinkedTreeGO::Node<fm::GameObjectID_t> *node);

		void _PurgeTree(LinkedTreeGO::Node<fm::GameObjectID_t>* parent, const std::shared_ptr<fm::Scene> inScene);

		std::optional<size_t>	_gameObjectSelected;
		std::string				_currentSceneName;
		bool					_isRenaming;
		char					_bufferName[128];
		bool					_goSelectedHasChanged = false;
		bool					_shouldUpdateListOrder = false;
		LinkedTreeGO			_order;


	};
}

