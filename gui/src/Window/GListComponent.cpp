#include "GListComponent.hpp"
#include <FML/Core/GameObject.h>
#include <FML/Components/Components.h>
#include <FML/Core/application.h>
#include <FML/Core/Scene.h>

#include "inspector/inspector.hpp"
#include "inspector/transformInspector.hpp"
#include "inspector/materialInspector.hpp"
#include "inspector/meshInspector.hpp"
#include "inspector/colliderInspector.hpp"
#include "inspector/cameraInspector.hpp"
#include "inspector/scriptManagerInspector.hpp"
#include "inspector/pointLightInspector.hpp"
#include "inspector/bodyInspector.hpp"

#include "inspector/textInspector.hpp"
#include <entt/entt.hpp>

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

	if (_inspectorComponents.find(currentEntity->GetID()) == _inspectorComponents.end())
	{
		_inspectorComponents[currentEntity->GetID()] = std::unordered_map<size_t, std::unique_ptr<Inspector>>();
	}
	auto &&inspectorComponent = _inspectorComponents[currentEntity->GetID()];

	for (size_t i = 0; i < fmc::ComponentType::kEND; ++i)
	{
		uint16_t componentType = i;
		auto &compo = inspectorComponent[componentType];
		if (compo == nullptr)
		{
			if (componentType == fmc::ComponentType::kTransform)
			{
				compo = std::make_unique<gui::TransformInspector>();
			}
			else if (componentType == fmc::ComponentType::kMaterial)
			{
				compo = std::make_unique <gui::MaterialInspector>();
			}
			else if (componentType == fmc::ComponentType::KMesh)
			{
				compo = std::make_unique <gui::MeshInspector>();
			}
			else if (componentType == fmc::ComponentType::kScriptManager)
			{
				compo = std::make_unique <gui::ScriptManagerInspector>();
			}
			else if (componentType == fmc::ComponentType::kPointLight)
			{
				compo = std::make_unique <gui::PointLightInspector>();
			}
			else if (componentType == fmc::ComponentType::kBody)
			{
				compo = std::make_unique <gui::BodyInspector>();
			}
			else if (componentType == fmc::ComponentType::kCollider)
			{
				compo = std::make_unique <gui::ColliderInspector>();
			}
			else if (componentType == fmc::ComponentType::kCamera)
			{
				compo = std::make_unique <gui::CameraInspector>();
			}
			else if (componentType == fmc::ComponentType::kText)
			{
				compo = std::make_unique <gui::TextInspector>();
			}
		}
		else
		{
			bool value = true;

			compo->Draw(&value, currentEntity);

			if (!value)
			{
				compo->RemoveComponent(currentEntity);

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
						_inspectorComponents[std::any_cast<fm::GameObjectID_t>(inEvent.value)].clear();
					}
				}
			});
		NeedUpdate();

	}
}
