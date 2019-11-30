#pragma once
#include <memory>
#include <vector>
#include "component.h"
#include <nlohmann/json_fwd.hpp>

namespace fm
{
class Script;
class FilePath;
}
class Entity;


namespace fmc {
	typedef std::vector<std::shared_ptr<fm::Script> > Scripts;
class CScriptManager : public FMComponent<CScriptManager> {

    public:
        CScriptManager();
        ~CScriptManager();
        void init(Entity* e);
        void update(Entity *e, float dt);
		void Start(Entity* e);
		void Stop(Entity* e);
		void addScriptLua(const fm::FilePath &inpath);

        void RemoveScript(const std::string &name);
        virtual uint16_t GetType() const {return kScriptManager;}

		bool Serialize(nlohmann::json &ioJson) const override;
		bool Read(const nlohmann::json &inJSON) override;


        void Destroy();
		void ReloadScript(const std::string &inName);
		Scripts GetScripts() const { return _scripts; }
private:
        Scripts _scripts;
};
}
