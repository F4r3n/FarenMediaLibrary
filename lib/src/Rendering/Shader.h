#pragma once
#include <string>

#include <Core/Config.h>

#include "Resource/Resource.h"
#include "Core/Math/Vector2.h"
#include "Core/Color.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Vector3.h"
#include <Resource/Resource.h>

namespace fm {

	class MaterialValue;
	class Color;

class Shader : public Resource{
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
    GLuint Program;
    Shader();
    
    Shader(const std::string& vertexCode, const std::string& fragmentCode, const std::string &name);
    const Shader* setValue(const std::string& name, fm::math::mat matrix) const;
    const Shader* setValue(const std::string& name, fm::math::vec2 vector) const;
    const Shader* setValue(const std::string& name, fm::math::vec3 vector) const;
    const Shader* setValue(const std::string& name, fm::math::vec4 vector) const;
    const Shader* setValue(const std::string& name, float val) const;
    const Shader* setValue(const std::string& name, int val) const;
    const Shader* setValue(const std::string& name, const Color &vector) const;
    const Shader* SetUniformBuffer(const std::string &name, unsigned int bindingPoint) const;

    const Shader* Use() const;
    bool compile();
    ~Shader();
    bool IsReady() const{return _isReady;}
    static constexpr fm::RESOURCE_TYPE getType() {return fm::RESOURCE_TYPE::SHADER;}
    void  setValue(const std::string &name, const fm::MaterialValue &value) const;
    const std::string& GetName() const{return _name;}
private:
    std::string _vertex, _fragment;
    ZTEST _zTest = ZTEST::LESS;
    BLEND _blendMode = BLEND::NONE;
    bool _isReady = false;
    std::string _name;
};
}
