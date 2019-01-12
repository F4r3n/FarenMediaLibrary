#ifndef SCRIPTMANAGERINSPECTOR_HPP
#define SCRIPTMANAGERINSPECTOR_HPP
#include "macroInspectorHelper.hpp"
#include "inspector/inspector.hpp"
#include "Components/CScriptManager.h"
namespace gui {
    DECLARE_INSPECTOR_CLASS(ScriptManager, fmc::CScriptManager)
    char nameScript[128] = "ClassName";

};
}
#endif // SCRIPTMANAGERINSPECTOR_HPP

