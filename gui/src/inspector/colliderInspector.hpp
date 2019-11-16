#pragma once
#include "inspector.hpp"
#include "Components/CCollider.h"
#include "macroInspectorHelper.hpp"
#include <vector>
namespace gui {
	DECLARE_INSPECTOR_CLASS(Collider, fmc::CCollider)
	int _currentShape;
	fm::math::vec3 _currentScale;
};
}
