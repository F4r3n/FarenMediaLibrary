#pragma once
#include <string>

#include <Core/Config.h>

#include "Resource/Resource.h"
#include "Core/Math/Vector2.h"
#include "Core/Color.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Vector3.h"
#include <Resource/Resource.h>
#include <filesystem>

namespace fm {

	class MaterialValue;
	class Color;

class Shader : public Resource
{
    enum ZTEST {
        ALWAYS,
        GREATER,
        LESS,
        EQUAL,
        NEVER
    };


    enum BLEND {
        ADD,
        MULT,
        NONE
    };
public:
    Shader();
    
    Shader(const fm::FilePath& inFilePath, const std::string &name);
    const Shader* setValue(const std::string& name, fm::math::mat matrix) const;
    const Shader* setValue(const std::string& name, fm::math::vec2 vector) const;
    const Shader* setValue(const std::string& name, fm::math::vec3 vector) const;
    const Shader* setValue(const std::string& name, fm::math::vec4 vector) const;
    const Shader* setValue(const std::string& name, float val) const;
    const Shader* setValue(const std::string& name, int val) const;
    const Shader* setValue(const std::string& name, const Color &vector) const;
    const Shader* SetUniformBuffer(const std::string &name, unsigned int bindingPoint) const;
	GLuint GetUniformBlockIndex(const std::string& name) const;

    const Shader* Use() const;
    bool compile();
    ~Shader();
    bool IsReady() const{return _isReady;}
    static fm::RESOURCE_TYPE getType() {return fm::RESOURCE_TYPE::SHADER;}
    void  setValue(const std::string &name, const fm::MaterialValue &value) const;
    const std::string& GetName() const{return _name;}

	void Reload(bool force = false);

private:
	std::filesystem::file_time_type _lastTimeFrag;
	std::filesystem::file_time_type _lastTimeVert;

	GLuint _program;

    bool _isReady = false;
    std::string _name;
};
}
