#include "inspector/materialInspector.hpp"
#include <imgui/imgui.h>
#include <Resource/ResourcesManager.h>
#include "Window/GMaterialEditor.h"
#include <FML/Core/GameObject.h>
#include <FML/Components/CMaterial.h>
using namespace gui;


MaterialInspector::MaterialInspector()
{
}


void MaterialInspector::Draw(bool *value,  std::shared_ptr<fm::GameObject> inGameObject)
{
	fmc::CMaterial& inTarget = inGameObject->get<fmc::CMaterial>();

	const std::string name = "Material##" + std::to_string(inGameObject->GetID());

    if(ImGui::CollapsingHeader(name.c_str(), value))
    {
        for(auto &m : inTarget.GetAllMaterials())
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

void MaterialInspector::RemoveComponent(std::shared_ptr<fm::GameObject> inGameObject)
{
	inGameObject->remove<fmc::CMaterial>();
}