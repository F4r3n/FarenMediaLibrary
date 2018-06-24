#pragma once
#include <vector>
#include "component.h"

namespace fm
{
class Script;
}
class Entity;


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


        static const std::string name;
        void Destroy();
        std::vector<std::shared_ptr<fm::Script> > scripts;
};
}
