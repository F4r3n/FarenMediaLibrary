#pragma once
#include "inspector.hpp"
#include "Components/CMaterial.h"
#include "macroInspectorHelper.hpp"
#include <vector>
namespace gui {
    DECLARE_INSPECTOR_CLASS(Material, fmc::CMaterial)
	std::vector<const char*> valuesShader;

	std::vector<const char*> typesMaterial;
};
}
