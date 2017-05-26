#pragma once
#include <string>
#include <fstream>
#include <sstream>


#include <GL/glew.h>

#include "Resource/Resource.h"
#include "Core/Math/Vector2.h"
#include "Core/Color.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Vector3.h"
namespace fm {
    

class Shader {
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
    Shader* setMatrix(const std::string& name, fm::math::mat matrix);

    Shader* setVector2f(const std::string& name, fm::math::vec2 vector);
    Shader* setVector3f(const std::string& name, fm::math::vec3 vector);
    Shader* setVector4f(const std::string& name, fm::math::vec4 vector);
    Shader* setFloat(const std::string& name, float val);
    Shader* setInt(const std::string& name, int val);
    
    Shader* setColor(const std::string& name, Color vector);
    
    Shader* Use();
    bool compile();
    ~Shader();

private:
    std::string vertex, fragment;
    ZTEST zTest = ZTEST::LESS;
    BLEND blendMode = BLEND::NONE;
};
}
