#pragma once
#include <vector>
#include "Component.h"
#include "Script/LuaScript.h"
#include "Entity.h"

namespace fmc {
class CScriptManager : public Component<CScriptManager> {
public:
    CScriptManager();
    ~CScriptManager();
    void init(Entity* e);
    void update();
    void addScript(std::shared_ptr<fm::Script> file);

template <typename T>
    void event(std::string name, const T& t) {
        for(auto s : scripts) {
            s->event(name, t);
        }
    }
    static const std::string name;
    int* get(int v) {return nullptr;}

private:
    std::vector<std::shared_ptr<fm::Script> > scripts;
};
}