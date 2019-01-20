#pragma once
#include "inspector.hpp"
#include "Components/CBody3D.h"
#include "macroInspectorHelper.hpp"
#include <vector>
namespace gui {
	DECLARE_INSPECTOR_CLASS(Body3D, fmc::CBody3D)
	int _currentShape;
	float _currentMass;
	float _currentRadius;
	fm::math::vec3 _currentScale;
};
}
