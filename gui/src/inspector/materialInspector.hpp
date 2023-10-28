#pragma once
#include "inspector.hpp"
#include "Components/CMaterial.h"
#include "macroInspectorHelper.hpp"
#include <vector>
#include "Rendering/MaterialValue.h"
#include "Window/GMaterialEditor.h"
namespace gui {
    DECLARE_INSPECTOR_CLASS(Material, fmc::CMaterial)
private:
	std::vector<const char*> _valuesShader;
	GMaterialEditor _editor;
};
}
