#pragma once

class ShaderLibrary 
{
 public:
	ShaderLibrary();
	~ShaderLibrary();
	static void LoadShaders();
	static void LoadShader(const std::string& name, const std::string &path );
};