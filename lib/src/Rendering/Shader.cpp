
#include "Rendering/Shader.h"
#include <iostream>
#include "Rendering/MaterialValue.h"
#include "Core/Color.h"
using namespace fm;

Shader::Shader():Resource(fm::FilePath("")) {
}

Shader::Shader(const fm::FilePath& inFilePath, const std::string& name) : Resource(inFilePath)
{
    _name = name;
}

bool Shader::compile() 
{
	File frag(Folder(_path), _path.GetName(true) + ".frag");
	File vert(Folder(_path), _path.GetName(true) + ".vert");

	const std::string vertContent = vert.GetContent();
	const std::string fragContent = frag.GetContent();

    const GLchar* vShaderCode = vertContent.c_str();
    const GLchar* fShaderCode = fragContent.c_str();

    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        std::cerr << vShaderCode << std::endl;
        return false;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        std::cerr << fShaderCode << std::endl;
        return false;
    }
    
    // Shader Program
    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);
    // Print linking errors if any
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return false;
    }
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    _isReady = true;
    return true;
}

const Shader* Shader::Use() const {
    glUseProgram(this->Program);
    return this;
}

const Shader* Shader::setValue(const std::string& name, fm::math::mat matrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(Program, name.c_str()), 1, GL_FALSE, fm::math::value_ptr(matrix));
    return this;
}

const Shader* Shader::setValue(const std::string& name, fm::math::vec2 vector) const
{
    glUniform2f(glGetUniformLocation(Program, name.c_str()), vector.x, vector.y);
    return this;
}

const Shader* Shader::setValue(const std::string& name, fm::math::vec3 vector) const
{
    glUniform3f(glGetUniformLocation(Program, name.c_str()), vector.x, vector.y, vector.z);
    return this;
}

const Shader* Shader::setValue(const std::string& name, float val) const
{
    glUniform1f(glGetUniformLocation(Program, name.c_str()), val);
    return this;
}

const Shader* Shader::setValue(const std::string& name, fm::math::vec4 vector) const
{
    glUniform4f(glGetUniformLocation(Program, name.c_str()), vector.x, vector.y, vector.z, vector.w);
    return this;
}

const Shader* Shader::setValue(const std::string& name, int val) const
{
    glUniform1i(glGetUniformLocation(Program, name.c_str()), val);
    return this;
}

const Shader* Shader::setValue(const std::string& name, const Color &vector) const
{
    return setValue(name, fm::math::vec4(vector.r, vector.g, vector.b, vector.a));
}

void Shader::setValue(const std::string &name, const fm::MaterialValue &value) const
{
    if(value.getType() == fm::ValuesType::VALUE_FLOAT)
    {
        setValue(name, value.getFloat());
    }else if(value.getType() == fm::ValuesType::VALUE_INT)
    {
        setValue(name, value.getInt());
    }
    else if(value.getType() == fm::ValuesType::VALUE_VECTOR2_FLOAT)
    {
        setValue(name, value.getVector2());
    }
    else if(value.getType() == fm::ValuesType::VALUE_VECTOR3_FLOAT)
    {
        setValue(name, value.getVector3());
    }
    else if(value.getType() == fm::ValuesType::VALUE_VECTOR4_FLOAT)
    {
        setValue(name, value.getVector4());
    }
    else if(value.getType() == fm::ValuesType::VALUE_MATRIX_FLOAT)
    {
        setValue(name, value.getMatrix());
    }
    else if(value.getType() == fm::ValuesType::VALUE_COLOR)
    {
            setValue(name, value.getColor());
    }
    else if(value.getType() == fm::ValuesType::VALUE_TEXTURE)
    {
        TextureMat t = value.getTexture();
        setValue("texture" + std::to_string(t.position), t.position);
        glActiveTexture(t.position);
        t.texture.bind();
    }
}

const Shader* Shader::SetUniformBuffer(const std::string &name, unsigned int bindingPoint) const
{
    int b = glGetUniformBlockIndex(Program, name.c_str());
	if(b !=-1)
		glUniformBlockBinding(Program, b, bindingPoint);
    return this;

}


Shader::~Shader() {
}
