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
#include "Core/application.h"
#include "Core/Scene.h"
using namespace gui;

ListComponentWindow::ListComponentWindow() : GWindow("Inspector", true)
{
	_enabled = true;
	_kind = gui::WINDOWS::WIN_INSPECTOR;
}

ListComponentWindow::~ListComponentWindow()
{

}

void ListComponentWindow::_Update(float, Context &inContext)
{
	if(inContext.currentGameObjectSelected.has_value())
		_currentGameObjectSelected = inContext.currentGameObjectSelected;
}

void ListComponentWindow::_Draw()
{
	if (_currentGameObjectSelected.has_value())
	{
		fm::GameObject* go = fm::Application::Get().GetCurrentScene()->GetGameObjectByID(_currentGameObjectSelected.value());
		if (go != nullptr)
		{
			ImGui::Text(go->GetName().c_str());
			_DrawComponents(go);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("popup from button");

			if (ImGui::BeginPopup("popup from button"))
			{
				ImGui::MenuItem("Components", nullptr, false, false);

				if (!go->has<fmc::CTransform>() && ImGui::MenuItem("Transform"))
				{
					go->add<fmc::CTransform>();
				}
				if (!go->has<fmc::CMesh>() && ImGui::MenuItem("Mesh"))
				{
					go->add<fmc::CMesh>();
				}
				if (!go->has<fmc::CMaterial>() && ImGui::MenuItem("Material"))
				{
					go->add<fmc::CMaterial>();
				}
				if (!go->has<fmc::CScriptManager>() && ImGui::MenuItem("ScriptManager"))
				{
					go->add<fmc::CScriptManager>();
				}
				if (!go->has<fmc::CPointLight>() && ImGui::MenuItem("PointLight"))
				{
					go->add<fmc::CPointLight>();
				}
				if (!go->has<fmc::CCollider3D>() && ImGui::MenuItem("Collider3D"))
				{
					go->add<fmc::CCollider3D>();
				}
				if (!go->has<fmc::CBody3D>() && ImGui::MenuItem("Body3D"))
				{
					go->add<fmc::CBody3D>();
				}
				if (!go->has<fmc::CCollider2D>() && ImGui::MenuItem("Collider2D"))
				{
					go->add<fmc::CCollider2D>();
				}
				if (!go->has<fmc::CBody2D>() && ImGui::MenuItem("Body2D"))
				{
					go->add<fmc::CBody2D>();
				}
				ImGui::EndPopup();
			}
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
			else if (componentType == fmc::ComponentType::kCollider3D)
			{
				compo = std::make_unique <gui::Collider3DInspector>(c);
			}
			else if (componentType == fmc::ComponentType::kCamera)
			{
				compo = std::make_unique <gui::CameraInspector>(c);
			}
		}
		else
		{
			bool value = true;

			compo->Draw(&value);

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
	_currentGameObjectSelected.reset();
}


void ListComponentWindow::CustomDraw()
{
	_Draw();
}