#ifndef SCRIPTMANAGERINSPECTOR_HPP
#define SCRIPTMANAGERINSPECTOR_HPP
#include "MacroInspectorHelper.h"
#include "inspector/Inspector.hpp"
#include "Components/CScriptManager.h"
namespace gui {
    DECLARE_INSPECTOR_CLASS(ScriptManager, fmc::CScriptManager)
    char nameScript[128] = "ClassName";

};
}
#endif // SCRIPTMANAGERINSPECTOR_HPP

