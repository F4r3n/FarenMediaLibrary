#pragma once
#include <string>
#include "Rendering/Shader.h"

namespace fm
{


class OGLShader : public fm::Shader
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
	OGLShader();

	OGLShader(const fm::FilePath& inFilePath, const std::string& name);

	virtual const fm::Shader* Use() const;
	virtual const fm::Shader* setValue(const std::string& name, fm::math::mat matrix) const;
	virtual const fm::Shader* setValue(const std::string& name, fm::math::vec2 vector) const;
	virtual const fm::Shader* setValue(const std::string& name, fm::math::vec3 vector) const;
	virtual const fm::Shader* setValue(const std::string& name, fm::math::vec4 vector) const;
	virtual const fm::Shader* setValue(const std::string& name, float val) const;
	virtual const fm::Shader* setValue(const std::string& name, int val) const;
	virtual const fm::Shader* setValue(const std::string& name, const fm::Color& vector) const;
	virtual void  setValue(const std::string& name, const fm::MaterialValue& value) const;

	void SetUniformBuffer(const std::string& name, unsigned int bindingPoint) const;

	virtual bool compile();
	virtual ~OGLShader();

	virtual void Reload(bool force = false);

private:
	GLuint _program;
};

}