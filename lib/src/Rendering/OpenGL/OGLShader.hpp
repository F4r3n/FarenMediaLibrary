#pragma once
#include <string>
#include "Rendering/SubShader.hpp"

namespace fm
{


	class OGLShader
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

		OGLShader(const fm::SubShader& inSubShader);

		const fm::OGLShader* Use() const;
		const fm::OGLShader* setValue(const std::string& name, fm::math::mat matrix) const;
		const fm::OGLShader* setValue(const std::string& name, fm::math::vec2 vector) const;
		const fm::OGLShader* setValue(const std::string& name, fm::math::vec3 vector) const;
		const fm::OGLShader* setValue(const std::string& name, fm::math::vec4 vector) const;
		const fm::OGLShader* setValue(const std::string& name, float val) const;
		const fm::OGLShader* setValue(const std::string& name, int val) const;
		const fm::OGLShader* setValue(const std::string& name, const fm::Color& vector) const;
		void  setValue(const std::string& name, const fm::MaterialValue& value) const;

		void SetUniformBuffer(const std::string& name, unsigned int bindingPoint) const;

		bool compile();

		void Reload(bool force = false);
		~OGLShader();
	private:
		std::optional<fm::SubShader> _subShader;
		unsigned int _program;
		std::filesystem::file_time_type _lastTimeFrag;
		std::filesystem::file_time_type _lastTimeVert;
		fm::FilePath	_path;
		bool _isReady = false;
	};

}