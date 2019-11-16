#pragma once
#include "inspector.hpp"
#include "Components/CBody3D.h"
#include "macroInspectorHelper.hpp"
#include <vector>
namespace gui {
	DECLARE_INSPECTOR_CLASS(Body3D, fmc::CBody3D)
	float _currentMass;
	bool _currentGhost;
};
}
