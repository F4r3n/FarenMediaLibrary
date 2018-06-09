#pragma once
#include <vector>
#include "component.h"
#include "Script/LuaScript.h"
#include "Entity.h"



namespace fmc {
class CScriptManager : public FMComponent<CScriptManager> {

    public:
        CScriptManager();
        ~CScriptManager();
        void init(Entity* e);
        void update(Entity *e);
        void addScript(std::shared_ptr<fm::Script> file);
        void RemoveScript(const std::string &name);
        virtual size_t GetType() const {return kScriptManager;}

        template <typename T>
        void event(std::string name, const T& t) {
            for(auto s : scripts) {
                s->event(name, t);
            }
        }
        static const std::string name;


        std::vector<std::shared_ptr<fm::Script> > scripts;
};
}
