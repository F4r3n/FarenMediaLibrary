#ifndef POINTLIGHTINSPECTOR_H
#define POINTLIGHTINSPECTOR_H

#include "inspector.hpp"
#include "Components/CPointLight.h"

#include "macroInspectorHelper.hpp"

namespace gui {
    DECLARE_INSPECTOR_CLASS(PointLight, fmc::CPointLight)

};
}

#endif // POINTLIGHTINSPECTOR_H
