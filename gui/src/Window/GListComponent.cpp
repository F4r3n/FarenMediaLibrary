#include "GListComponent.hpp"
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
#include "inspector/bodyInspector.hpp"
#include "Core/application.h"
#include "Core/Scene.h"
#include "inspector/textInspector.hpp"
#include "EntityManager.h"
#include "Entity.h"
using namespace gui;

GListComponent::GListComponent() : GWindow("Inspector", true)
{
	_enabled = true;
	_kind = gui::WINDOWS::WIN_INSPECTOR;
}

GListComponent::~GListComponent()
{

}

void GListComponent::_Update(float, Context &inContext)
{
	if(inContext.currentGameObjectSelected.has_value())
		_currentGameObjectSelected = inContext.currentGameObjectSelected;
}

void GListComponent::_Draw()
{
	if (_currentGameObjectSelected.has_value())
	{
		std::shared_ptr<fm::GameObject> go = fm::Application::Get().GetCurrentScene()->GetGameObjectByID(_currentGameObjectSelected.value());
		if (go != nullptr)
		{
			ImGui::Text("%s", go->GetName().c_str());
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
				if (!go->has<fmc::CCollider>() && ImGui::MenuItem("Collider"))
				{
					go->add<fmc::CCollider>();
				}
				if (!go->has<fmc::CBody>() && ImGui::MenuItem("Body"))
				{
					go->add<fmc::CBody>();
				}
				if (!go->has<fmc::CText>() && ImGui::MenuItem("Text"))
				{
					go->add<fmc::CText>();
				}
				if (!go->has<fmc::CText>() && ImGui::MenuItem("Camera"))
				{
					go->add<fmc::CCamera>();
				}

				ImGui::EndPopup();
			}
		}

	}
}

void GListComponent::_DrawComponents(std::shared_ptr<fm::GameObject> currentEntity)
{

	std::vector<BaseComponent*> &&compos = currentEntity->getAllComponents();
	if (_inspectorComponents.find(currentEntity->getID().id()) == _inspectorComponents.end())
	{
		_inspectorComponents[currentEntity->getID().id()] = std::unordered_map<size_t, std::unique_ptr<Inspector>>();
	}
	auto &&inspectorComponent = _inspectorComponents[currentEntity->getID().id()];

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
			else if (componentType == fmc::ComponentType::kBody)
			{
				compo = std::make_unique <gui::BodyInspector>(c);
			}
			else if (componentType == fmc::ComponentType::kCollider)
			{
				compo = std::make_unique <gui::ColliderInspector>(c);
			}
			else if (componentType == fmc::ComponentType::kCamera)
			{
				compo = std::make_unique <gui::CameraInspector>(c);
			}
			else if (componentType == fmc::ComponentType::kText)
			{
				compo = std::make_unique <gui::TextInspector>(c);
			}
		}
		else
		{
			bool value = true;
			auto& manager = ::EntityManager::get();
			compo->Draw(&value, manager.GetEntity(currentEntity->getID()));

			if (!value)
			{
				compo->RemoveComponent(manager.GetEntity(currentEntity->getID()));

				compo.reset();
				inspectorComponent[componentType] = nullptr;
			}
		}
	}
}

void GListComponent::_ClearInspectorComponents()
{
	for (auto&& entities : _inspectorComponents)
	{
		entities.second.clear();
	}
	_inspectorComponents.clear();
	_currentGameObjectSelected.reset();
}


void GListComponent::CustomDraw()
{
	_Draw();
}


void GListComponent::OnBeforeLoad(const std::string& inCurrentSceneName)
{
	std::shared_ptr<fm::Scene> oldScene = fm::Application::Get().GetScene(inCurrentSceneName);
	if (oldScene != nullptr)
		oldScene->Unsubscribe(this);

	_ClearInspectorComponents();
}


void GListComponent::OnAfterLoad(const std::string& inCurrentSceneName)
{
	std::shared_ptr<fm::Scene> newScene = fm::Application::Get().GetScene(inCurrentSceneName);
	if (newScene != nullptr)
		newScene->Subscribe(this);
}


void GListComponent::Notify(fm::Observable* o, const fm::EventObserver& inEvent)
{
	if (o->GetName() == "Scene")
	{
		AddEvent([inEvent, this](gui::GWindow* inWindow, std::optional<gui::Context> Context)
			{
				if (inEvent.eventKind == (size_t)fm::Scene::Event::DELETE_GAMEOBJECT)
				{
					if (inEvent.value.has_value())
					{
						_inspectorComponents[std::any_cast<Entity::Id>(inEvent.value).id()].clear();
					}
				}
			});
		NeedUpdate();

	}
}
