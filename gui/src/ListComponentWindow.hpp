#pragma once
#include "GWindow.h"
#include <memory>
#include <unordered_map>
namespace fm
{
	class GameObject;
}

class Inspector;

namespace gui
{
	typedef std::unordered_map<size_t, std::unordered_map<size_t, std::unique_ptr<Inspector>>> MapOfInspectors;

	class ListComponentWindow : public GWindow
	{


	public:
		ListComponentWindow();
		~ListComponentWindow();
		virtual void CustomDraw() override;
		void ClearInspectorComponents();

	private:
		void _Draw();

		void _DrawComponents(fm::GameObject* currentEntity);
		virtual void _Update(float, Context &inContext) override;

	private:
		fm::GameObject*				_currentGameObjectSelected;
		MapOfInspectors				_inspectorComponents;

	};
}