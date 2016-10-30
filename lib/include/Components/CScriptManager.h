#pragma once
#include <vector>
#include "Component.h"
#include <single/sol/sol.hpp>
#include "Script.h"
#include "Entity.h"
namespace fmc {
class CScriptManager : public Component<CScriptManager> {
public:
    CScriptManager();
    ~CScriptManager();
    void init(sol::state& lua, Entity* e);
    void update(float dt, sol::state& lua);
    void addScript(std::shared_ptr<fm::Script> file);

    template <typename T> void event(std::string name, sol::state& lua, const T& t) {
        for(auto s : scripts) {
            s->event(name, lua, t);
        }
    }
    static const std::string name;
#ifdef GUI
    void display(bool* value) {
        if(ImGui::CollapsingHeader(name, value)) {

          
        }
    }
#endif
private:
    std::vector<std::shared_ptr<fm::Script> > scripts;
};
}