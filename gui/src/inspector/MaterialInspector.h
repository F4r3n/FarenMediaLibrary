#pragma once
#include "Inspector.hpp"
#include "Components/CMaterial.h"

#include "MacroInspectorHelper.h"

namespace gui {
    DECLARE_INSPECTOR_CLASS(Material, fmc::CMaterial)
        char shaderName[128];
    char buf1[64];
};
}
