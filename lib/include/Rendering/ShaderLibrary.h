#pragma once
#include "Resource/ResourcesManager.h"

class ShaderLibrary 
{
 public:
	ShaderLibrary();
	~ShaderLibrary();
	static void loadShaders();
	static void loadShader(const std::string& name, const std::string &path );
};