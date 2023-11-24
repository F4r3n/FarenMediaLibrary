#pragma once
#if defined(_MSC_VER) && _MSC_VER >= 1900
#pragma warning(disable: 4100)
#endif
#include <string>
#include <Core/FilePath.h>
#include <nlohmann/json_fwd.hpp>
#include <entt/entt.hpp>

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
		entt::handle entity;
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
    virtual ~Script() {}
	virtual bool init() = 0;
	virtual void start() = 0;
	virtual void update(float) = 0;
	virtual void Stop(const entt::handle& e) = 0;
	virtual bool Serialize(nlohmann::json &ioJson) const = 0;
	virtual bool Read(const nlohmann::json &inJSON) = 0;

	virtual SCRIPT_TYPE GetType() const = 0;

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
