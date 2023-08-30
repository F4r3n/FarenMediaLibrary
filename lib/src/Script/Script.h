#pragma once
#pragma warning(disable: 4100)
#include <string>
#include <Core/FilePath.h>
#include <nlohmann/json_fwd.hpp>
#include "Entity.h"
class Entity;

namespace fm {
class CppScript;
class BaseEvent;
}
namespace fmc
{
	class CTransform;
}

class Behaviour
{
        friend class fm::CppScript;

    public:
        Behaviour() {}
        virtual ~Behaviour() {}
        virtual void Update() {}
        virtual void Start() {}

		fmc::CTransform* GetTransform() { return nullptr; }
    protected:
        Entity entity;
};

namespace fm {


class Script {


public:
    enum SCRIPT_TYPE
    {
        LUA,
        CPP,
        NONE
    };


    Script() {}
    ~Script() {}
	virtual bool init() = 0;
	virtual void start() = 0;
	virtual void update(float) = 0;
	virtual void Stop(const Entity& e) = 0;
	virtual bool Serialize(nlohmann::json &ioJson) const = 0;
	virtual bool Read(const nlohmann::json &inJSON) = 0;

	virtual SCRIPT_TYPE GetType() const = 0;
	virtual void CallEvent(fm::BaseEvent* inEvent) {}

	virtual bool Reload(bool inCreateInstance) { return false; }

	const fm::File& GetFile() const { return _file; }
	const std::string& GetScriptName() const { return _scriptName; }

	bool HasStarted() const { return _hasStarted; }
   protected:
    std::string		_scriptName;
	bool			_hasStarted;
	bool			_isInit;
	fm::File		_file;

private:

};
}
