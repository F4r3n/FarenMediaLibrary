#pragma once
#include <memory>
#include <vector>
#include "component.h"

namespace fm
{
class Script;
class FilePath;
}
class Entity;


namespace fmc {
class CScriptManager : public FMComponent<CScriptManager> {

    public:
        CScriptManager();
        ~CScriptManager();
        void init(Entity* e);
        void update(Entity *e, float dt);
		void addScriptLua(const fm::FilePath &inpath);

        void RemoveScript(const std::string &name);
        virtual size_t GetType() const {return kScriptManager;}


        void Destroy();
		void ReloadScript(const std::string &inName);
        std::vector<std::unique_ptr<fm::Script> > scripts;
};
}
