#pragma once
#include <vector>
#include "Component.h"
#include <single/sol/sol.hpp>
#include "Script.h"
namespace fmc {
class CScriptManager : public Component<CScriptManager> {
public:
    CScriptManager();
    ~CScriptManager();
    void init(sol::state &lua);
    void update(float dt, sol::state &lua);
    void addScript(std::shared_ptr<fm::Script> file);

private:
    std::vector<std::shared_ptr<fm::Script> > scripts;
};
}