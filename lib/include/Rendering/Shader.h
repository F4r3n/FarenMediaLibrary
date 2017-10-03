#pragma once
#include <string>
#include <fstream>
#include <sstream>


#include <Core/Config.h>

#include "Resource/Resource.h"
#include "Core/Math/Vector2.h"
#include "Core/Color.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Vector3.h"
#include <Resource/Resource.h>
namespace fm {

   

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
    //Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    Shader(const std::string& vertexCode, const std::string& fragmentCode);
    Shader* setValue(const std::string& name, fm::math::mat matrix);
    Shader* setValue(const std::string& name, fm::math::vec2 vector);
    Shader* setValue(const std::string& name, fm::math::vec3 vector);
    Shader* setValue(const std::string& name, fm::math::vec4 vector);
    Shader* setValue(const std::string& name, float val);
    Shader* setValue(const std::string& name, int val);
    Shader* setValue(const std::string& name, Color vector);

    Shader* Use();
    bool compile();
    ~Shader();
    bool IsReady() const{return isReady;}
    static constexpr fm::RESOURCE_TYPE getType() {return fm::RESOURCE_TYPE::SHADER;}

private:
    std::string vertex, fragment;
    ZTEST zTest = ZTEST::LESS;
    BLEND blendMode = BLEND::NONE;
    bool isReady = false;
};
}
