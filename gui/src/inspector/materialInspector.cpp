#include "inspector/materialInspector.hpp"
#include <imgui/imgui.h>
#include <Resource/ResourcesManager.h>
#include <Rendering/Shader.h>
#include <Core/Debug.h>
#include <Rendering/material.hpp>
#include "Window/GMaterialEditor.h"
#include <ECS.h>
using namespace gui;
DEFINE_INSPECTOR_FUNCTIONS(Material, fmc::CMaterial)


void MaterialInspector::_DeInit()
{

}


void MaterialInspector::_Init()
{
    fm::Debug::get().LogError("InitMaterial");
    std::map<std::string, std::shared_ptr<fm::Resource>> resources = fm::ResourcesManager::get().getAll<fm::Shader>();
    for(auto &r : resources)
    {
        fm::Shader *s = static_cast<fm::Shader*>(r.second.get());
        _valuesShader.push_back(s->GetName().c_str());
    }


}

void MaterialInspector::Draw(bool *value, const Entity& e)
{
	std::string name = _target->GetName() + "##" + std::to_string(e.id().index());

    if(ImGui::CollapsingHeader(name.c_str(), value))
    {
        for(auto &m :_target->GetAllMaterials())
        {
            std::string materialName = m->GetName() + "##Material";

            if (ImGui::TreeNode(materialName.c_str()))
            {
                size_t j = 0;
				std::string shaderName = m->GetShader()->GetName();

				GMaterialEditor::DrawMaterialInspector(m.get());

				ImGui::Separator();

                if(ImGui::Button("Add"))
                {
                    m->setValue("NEW", (int)0);
                }

                //std::string currentItem = shaderName.c_str();
                //size_t index = 0;
				//if (DrawCombo("Shader##" + m->GetName(), _valuesShader, currentItem, &index))
				//{
				//	m->SetShader(fm::ResourcesManager::get().getResource<fm::Shader>(currentItem));
				//}

                ImGui::TreePop();
            }
        }
    }
}

void MaterialInspector::RemoveComponent(const Entity& inEntity)
{
	EntityManager::get().removeComponent<fmc::CMaterial>(inEntity.id());
}