#pragma once

#include "Rendering/Shader.h"
#include "vulkan/vulkan.h"

namespace fm
{
class VkShader : public Shader
{
public:
	enum class KIND
	{
		VERT,
		FRAG
	};
	VkShader();

	VkShader(const fm::FilePath& inFilePath, const std::string& name);

	virtual bool compile();
	~VkShader();

	bool Make(VkDevice inDevice);
	bool Delete(VkDevice inDevice);
	VkPipelineShaderStageCreateInfo GetStageCreateInfo(KIND inKind) const;
protected:


private:
	VkShaderModule _CreateShaderModule(const std::vector<char>& code, VkDevice inDevice) const;

	VkShaderModule _vertShaderModule = nullptr;
	VkShaderModule _fragShaderModule = nullptr;
};
}