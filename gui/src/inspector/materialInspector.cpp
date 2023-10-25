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
				std::string shaderName = m->GetShaderPath().GetName(true);

				_editor.DrawMaterialInspector(m);

                ImGui::TreePop();
            }
        }
    }
}

void MaterialInspector::RemoveComponent(const Entity& inEntity)
{
	EntityManager::get().removeComponent<fmc::CMaterial>(inEntity.id());
}