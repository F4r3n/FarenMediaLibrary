#include "imgui_user.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <string>
//Custom widgets
bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size)
{
    using namespace ImGui;
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImGuiID id = window->GetID("##Splitter");
    ImRect bb;
    bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
    bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
    return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
}

bool DrawCombo(const std::string& inNameCombo, const std::vector<std::string>& values, std::string& currentItem, size_t* index)
{
    const char* item_current_char = currentItem.c_str();
    std::string nameCombo = inNameCombo;
    if (ImGui::BeginCombo(nameCombo.c_str(), item_current_char, 0))
    {
        for (size_t n = 0; n < values.size(); n++)
        {
            bool is_selected = (item_current_char == values[n]);
            if (ImGui::Selectable(values[n].c_str(), is_selected))
            {
                item_current_char = values[n].c_str();
                *index = n;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    bool hasChanged = currentItem != std::string(item_current_char);
    currentItem = std::string(item_current_char);
    return hasChanged;
}


bool DrawCombo(const std::string& inNameCombo, const std::vector<std::string>& values, size_t* index)
{
    size_t beforeIndex = *index;
    const char* item_current_char = values[*index].c_str();
    std::string nameCombo = inNameCombo;
    if (ImGui::BeginCombo(nameCombo.c_str(), item_current_char, 0))
    {
        for (size_t n = 0; n < values.size(); n++)
        {
            bool is_selected = (item_current_char == values[n]);
            if (ImGui::Selectable(values[n].c_str(), is_selected))
            {
                item_current_char = values[n].c_str();
                *index = n;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    return beforeIndex != *index;
}
