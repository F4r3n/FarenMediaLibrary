#pragma once
#include "inspector.hpp"
#include <FML/Components/CMaterial.h>
#include "macroInspectorHelper.hpp"
#include <vector>
#include "Window/GMaterialEditor.h"
namespace gui {
    DECLARE_INSPECTOR_CLASS(Material, fmc::CMaterial)
private:
	std::vector<const char*> _valuesShader;
	GMaterialEditor _editor;
};
}
