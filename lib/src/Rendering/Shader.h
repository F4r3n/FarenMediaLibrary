#pragma once
#pragma warning(disable: 4100)
#include <string>

#include <Core/Config.h>

#include "Resource/Resource.h"
#include "Core/Math/Vector2.h"
#include "Core/Color.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Vector3.h"
#include <Resource/Resource.h>
#include <filesystem>
#include "MaterialValue.h"
namespace fm {

	class MaterialValue;
	class Color;

class Shader : public Resource
{
public:

	struct Uniform
	{
		std::string name;
		int binding = -1;
		int set = -1;
		fm::ValuesType type = fm::ValuesType::VALUE_NONE;
	};

	struct Reflection
	{
		std::map<std::string, Uniform> uniforms;
	};

    Shader();
    
    Shader(const fm::FilePath& inFilePath);
    
	virtual bool compile() { return false; }
    virtual ~Shader();
    bool IsReady() const{return _isReady;}
    static fm::RESOURCE_TYPE getType() {return fm::RESOURCE_TYPE::SHADER;}
	virtual fm::RESOURCE_TYPE GetType() const override { return getType(); }

    const std::string& GetName() const{return _name;}

	void Reload(bool force = false) { ; }

	//TODO To remove
	virtual const Shader* Use() const { return this; }
	virtual const Shader* setValue(const std::string& name, fm::math::mat matrix) const { return this; }
	virtual const Shader* setValue(const std::string& name, fm::math::vec2 vector) const { return this; }
	virtual const Shader* setValue(const std::string& name, fm::math::vec3 vector) const { return this; }
	virtual const Shader* setValue(const std::string& name, fm::math::vec4 vector) const { return this; }
	virtual const Shader* setValue(const std::string& name, float val) const { return this; }
	virtual const Shader* setValue(const std::string& name, int val) const { return this; }
	virtual const Shader* setValue(const std::string& name, const Color& vector) const { return this; }
	virtual void  setValue(const std::string& name, const fm::MaterialValue& value) const { ; }

	void Save(nlohmann::json& outJSON) const override;
	void Load(const nlohmann::json& inJSON) override;
	void SetReflection(const Shader::Reflection& inReflection) { _reflection = inReflection; }
protected:
	virtual bool _Load() { return false; }

	std::filesystem::file_time_type _lastTimeFrag;
	std::filesystem::file_time_type _lastTimeVert;

	bool _isReady = false;
private:
	Reflection _reflection;
    std::string _name;
};
}
