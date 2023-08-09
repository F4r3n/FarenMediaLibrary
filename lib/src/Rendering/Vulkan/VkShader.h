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

	virtual bool compile();
	~VkShader();

	bool Make(VkDevice inDevice);

protected:


private:
	VkShaderModule _CreateShaderModule(const std::vector<char>& code, VkDevice inDevice) const;
};
}