#pragma once
#include "Window/GWindow.h"
#include <memory>
#include <unordered_map>
#include <optional>
#include "Core/Observer.h"
#include <FML/Core/GameObjectType.hpp>

namespace fm
{
	class GameObject;
}

class Inspector;

namespace gui
{
	typedef std::unordered_map<fm::GameObjectID_t, std::unordered_map<size_t, std::unique_ptr<Inspector>>> MapOfInspectors;

	class GListComponent : public GWindow, public fm::Observer
	{


	public:
		GListComponent();
		~GListComponent();
		virtual void CustomDraw() override;

		void OnBeforeLoad(const std::string& inCurrentSceneName);
		void OnAfterLoad(const std::string& inCurrentSceneName);
		virtual void Notify(fm::Observable*, const fm::EventObserver& inEvent) override;

	private:
		void _Draw();
		void _ClearInspectorComponents();
		void _DrawComponents(std::shared_ptr<fm::GameObject> currentEntity);
		virtual void _Update(float, Context &inContext) override;

	private:
		std::optional<fm::GameObjectID_t>	_currentGameObjectSelected;
		MapOfInspectors						_inspectorComponents;

	};
}