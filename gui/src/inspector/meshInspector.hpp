#pragma once
#include "Inspector.hpp"
#include "Components/CMesh.h"

#include "macroInspectorHelper.hpp"

namespace gui {
    DECLARE_INSPECTOR_CLASS(Mesh, fmc::CMesh)
    int current = 0;

};
}
