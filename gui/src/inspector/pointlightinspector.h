#ifndef POINTLIGHTINSPECTOR_H
#define POINTLIGHTINSPECTOR_H

#include "Inspector.hpp"
#include "Components/CPointLight.h"

#include "MacroInspectorHelper.h"

namespace gui {
    DECLARE_INSPECTOR_CLASS(PointLight, fmc::CPointLight)

};
}

#endif // POINTLIGHTINSPECTOR_H
