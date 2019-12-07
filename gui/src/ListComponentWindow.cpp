#include "ListComponentWindow.hpp"
#include "Core/GameObject.h"
#include "Components/Components.h"

#include "inspector/inspector.hpp"
#include "inspector/transformInspector.hpp"
#include "inspector/materialInspector.hpp"
#include "inspector/meshInspector.hpp"
#include "inspector/colliderInspector.hpp"
#include "inspector/cameraInspector.hpp"
#include "inspector/scriptManagerInspector.hpp"
#include "inspector/pointLightInspector.hpp"
#include "inspector/body3DInspector.hpp"

using namespace gui;

ListComponentWindow::ListComponentWindow() : GWindow("Inspector", true)
{
	_enabled = true;
	_kind = gui::WINDOWS::WIN_INSPECTOR;
	_currentGameObjectSelected = nullptr;
}

ListComponentWindow::~ListComponentWindow()
{

}

void ListComponentWindow::_Update(float, Context &inContext)
{
	_currentGameObjectSelected = inContext.currentGameObjectSelected;
}

void ListComponentWindow::_Draw()
{
	if (_currentGameObjectSelected && _currentGameObjectSelected->IsActive())
	{
		ImGui::Text(_currentGameObjectSelected->GetName().c_str());
		_DrawComponents(_currentGameObjectSelected);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("popup from button");

		if (ImGui::BeginPopup("popup from button"))
		{
			ImGui::MenuItem("Components", nullptr, false, false);

			if (!_currentGameObjectSelected->has<fmc::CTransform>() && ImGui::MenuItem("Transform"))
			{
				_currentGameObjectSelected->add<fmc::CTransform>();
			}
			if (!_currentGameObjectSelected->has<fmc::CMesh>() && ImGui::MenuItem("Mesh"))
			{
				_currentGameObjectSelected->add<fmc::CMesh>();
			}
			if (!_currentGameObjectSelected->has<fmc::CMaterial>() && ImGui::MenuItem("Material"))
			{
				_currentGameObjectSelected->add<fmc::CMaterial>();
			}
			if (!_currentGameObjectSelected->has<fmc::CScriptManager>() && ImGui::MenuItem("ScriptManager"))
			{
				_currentGameObjectSelected->add<fmc::CScriptManager>();
			}
			if (!_currentGameObjectSelected->has<fmc::CPointLight>() && ImGui::MenuItem("PointLight"))
			{
				_currentGameObjectSelected->add<fmc::CPointLight>();
			}
			if (!_currentGameObjectSelected->has<fmc::CCollider>() && ImGui::MenuItem("Collider"))
			{
				_currentGameObjectSelected->add<fmc::CCollider>();
			}
			if (!_currentGameObjectSelected->has<fmc::CBody3D>() && ImGui::MenuItem("Body3D"))
			{
				_currentGameObjectSelected->add<fmc::CBody3D>();
			}
			ImGui::EndPopup();
		}
	}
}

void ListComponentWindow::_DrawComponents(fm::GameObject* currentEntity)
{

	std::vector<BaseComponent*> &&compos = currentEntity->getAllComponents();
	if (_inspectorComponents.find(currentEntity->getID()) == _inspectorComponents.end())
	{
		_inspectorComponents[currentEntity->getID()] = std::unordered_map<size_t, std::unique_ptr<Inspector>>();
	}
	auto &&inspectorComponent = _inspectorComponents[currentEntity->getID()];

	for (auto && c : compos)
	{
		uint16_t componentType = c->GetType();
		auto &compo = inspectorComponent[componentType];
		if (compo == nullptr)
		{
			if (componentType == fmc::ComponentType::kTransform)
			{
				compo = std::make_unique<gui::TransformInspector>(c);
			}
			else if (componentType == fmc::ComponentType::kMaterial)
			{
				compo = std::make_unique <gui::MaterialInspector>(c);
			}
			else if (componentType == fmc::ComponentType::KMesh)
			{
				compo = std::make_unique <gui::MeshInspector>(c);
			}
			else if (componentType == fmc::ComponentType::kScriptManager)
			{
				compo = std::make_unique <gui::ScriptManagerInspector>(c);
			}
			else if (componentType == fmc::ComponentType::kPointLight)
			{
				compo = std::make_unique <gui::PointLightInspector>(c);
			}
			else if (componentType == fmc::ComponentType::kBody3D)
			{
				compo = std::make_unique <gui::Body3DInspector>(c);
			}
			else if (componentType == fmc::ComponentType::kCollider)
			{
				compo = std::make_unique <gui::ColliderInspector>(c);
			}
			else if (componentType == fmc::ComponentType::kCamera)
			{
				compo = std::make_unique <gui::CameraInspector>(c);
			}
		}
		else
		{
			bool value = true;

			compo->draw(&value);

			if (!value)
			{
				compo.reset();
				inspectorComponent[componentType] = nullptr;
				c->Destroy();
			}
		}
	}
}

void ListComponentWindow::ClearInspectorComponents()
{
	for (auto&& entities : _inspectorComponents)
	{
		entities.second.clear();
	}
	_inspectorComponents.clear();
	_currentGameObjectSelected = nullptr;
}


void ListComponentWindow::CustomDraw()
{
	_Draw();
}