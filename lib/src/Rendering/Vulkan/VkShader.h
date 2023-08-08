#pragma once

#include "Rendering/Shader.h"
#include "vulkan/vulkan.h"

namespace fm
{
class VkShader : public Shader
{
public:
	VkShader();

	VkShader(const fm::FilePath& inFilePath, const std::string& name);

	virtual bool compile() { return false; }
	virtual ~VkShader();
protected:
	virtual bool _Load();


private:
	VkShaderModule _CreateShaderModule(const std::vector<char>& code) const;
};
}