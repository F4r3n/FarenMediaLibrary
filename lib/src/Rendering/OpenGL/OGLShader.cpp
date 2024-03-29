#include "Rendering/Shader.h"
#include <iostream>
#include "Rendering/MaterialValue.h"
#include "Core/Color.h"
#include "OGLShader.hpp"
#include "GL/glew.h"
#include <cassert>
using namespace fm;


OGLShader::OGLShader(const fm::SubShader& inSubShader)
{
	_path = inSubShader.GetPath();
	_subShader = inSubShader;
}

void OGLShader::Reload(bool force)
{
	if (_isReady || force)
	{
		File frag(Folder(_path), _path.GetName(true) + ".frag");
		File vert(Folder(_path), _path.GetName(true) + ".vert");

		if (force || (_lastTimeFrag != frag.GetTimeStamp() || _lastTimeVert != vert.GetTimeStamp()))
		{
			_isReady = false;
			glDeleteShader(_program);
			_program = 0;
		}
	}
}



bool OGLShader::compile()
{
	fm::FilePath preprocessedShader(_path);
	preprocessedShader.ToSub("Preprocessed");
	File frag(Folder(preprocessedShader), "frag.frag");
	File vert(Folder(preprocessedShader), "vert.vert");
	if (!(frag.Exist() && vert.Exist()))
		return false;

	_lastTimeFrag = frag.GetTimeStamp();
	_lastTimeVert = vert.GetTimeStamp();

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
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		std::cerr << vShaderCode << std::endl;
		return false;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		std::cerr << fShaderCode << std::endl;
		return false;
	}

	// Shader Program
	_program = glCreateProgram();
	glAttachShader(_program, vertex);
	glAttachShader(_program, fragment);
	glLinkProgram(_program);
	// Print linking errors if any
	glGetProgramiv(_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(_program, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return false;
	}
	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	_isReady = true;

	int error = glGetError();
	if (error != 0) {
		std::cerr << "ERROR OPENGL " << error << " " << __LINE__ << " " << __FILE__ << std::endl;
	}
	return true;
}

const OGLShader* OGLShader::Use() const {
	glUseProgram(_program);
	return this;
}

const OGLShader* OGLShader::setValue(const std::string& name, fm::math::mat matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(_program, name.c_str()), 1, GL_FALSE, fm::math::value_ptr(matrix));
	return this;
}

const OGLShader* OGLShader::setValue(const std::string& name, fm::math::vec2 vector) const
{
	glUniform2f(glGetUniformLocation(_program, name.c_str()), vector.x, vector.y);
	return this;
}

const OGLShader* OGLShader::setValue(const std::string& name, fm::math::vec3 vector) const
{
	glUniform3f(glGetUniformLocation(_program, name.c_str()), vector.x, vector.y, vector.z);
	return this;
}

const OGLShader* OGLShader::setValue(const std::string& name, float val) const
{
	glUniform1f(glGetUniformLocation(_program, name.c_str()), val);
	return this;
}

const OGLShader* OGLShader::setValue(const std::string& name, fm::math::vec4 vector) const
{
	glUniform4f(glGetUniformLocation(_program, name.c_str()), vector.x, vector.y, vector.z, vector.w);
	return this;
}

const OGLShader* OGLShader::setValue(const std::string& name, int val) const
{
	glUniform1i(glGetUniformLocation(_program, name.c_str()), val);
	return this;
}

const OGLShader* OGLShader::setValue(const std::string& name, const Color& vector) const
{
	return setValue(name, fm::math::vec4(vector.r, vector.g, vector.b, vector.a));
}

void OGLShader::setValue(const std::string& name, const fm::MaterialValue& value) const
{
	if (value.getType() == fm::ValuesType::VALUE_FLOAT)
	{
		setValue(name, value.getFloat());
	}
	else if (value.getType() == fm::ValuesType::VALUE_INT)
	{
		setValue(name, value.getInt());
	}
	else if (value.getType() == fm::ValuesType::VALUE_VECTOR2_FLOAT)
	{
		setValue(name, value.getVector2());
	}
	else if (value.getType() == fm::ValuesType::VALUE_VECTOR3_FLOAT)
	{
		setValue(name, value.getVector3());
	}
	else if (value.getType() == fm::ValuesType::VALUE_VECTOR4_FLOAT)
	{
		setValue(name, value.getVector4());
	}
	else if (value.getType() == fm::ValuesType::VALUE_MATRIX_FLOAT)
	{
		setValue(name, value.getMatrix());
	}
	else if (value.getType() == fm::ValuesType::VALUE_COLOR)
	{
		setValue(name, value.getColor());
	}
}


void OGLShader::SetUniformBuffer(const std::string& name, unsigned int bindingPoint) const
{
	int b = glGetUniformBlockIndex(_program, name.c_str());
	if (b != -1)
		glUniformBlockBinding(_program, b, bindingPoint);
}


OGLShader::~OGLShader() {
}
