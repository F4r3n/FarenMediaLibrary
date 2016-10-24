#pragma once
#include "CTransform.h"
#include "Entity.h"
#include <imgui.h>


class ComponentInterface {
    public:
template <typename T>
static void display(T *t) {
     ImGui::TextWrapped("This window is being created by the ShowTestWindow() function. Please refer to the "
                                   "code for programming reference.\n\nUser Guide:");
}

};