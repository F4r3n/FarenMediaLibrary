#pragma once
#include "GWindow.h"
#include <memory>
#include <unordered_map>
#include <optional>
#include "Core/Observer.h"

namespace fm
{
	class GameObject;
}

class Inspector;

namespace gui
{
	typedef std::unordered_map<size_t, std::unordered_map<size_t, std::unique_ptr<Inspector>>> MapOfInspectors;

	class ListComponentWindow : public GWindow, public fm::Observer
	{


	public:
		ListComponentWindow();
		~ListComponentWindow();
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
		std::optional<ecs::id>		_currentGameObjectSelected;
		MapOfInspectors				_inspectorComponents;

	};
}