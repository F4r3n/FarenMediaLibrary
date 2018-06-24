#pragma once
#include "Inspector.hpp"
#include "Components/CMaterial.h"
#include "MacroInspectorHelper.h"
#include <vector>
namespace gui {
    DECLARE_INSPECTOR_CLASS(Material, fmc::CMaterial)
    std::vector<const char*> values;
    size_t lengthBuffer;
};
}
