#pragma once

#include "Rendering/Shader.h"
#include "vulkan/vulkan.h"
#include "Core/Math/Matrix.h"

namespace fm
{
class VkShader
{
public:
	enum class KIND
	{
		VERT,
		FRAG
	};
	struct MeshPushConstants {
		fm::math::vec4	data;
		fm::math::mat	render_matrix;
	};

	VkShader(const fm::FilePath& inFilePath);

	~VkShader();

	bool Make(VkDevice inDevice);
	bool Delete(VkDevice inDevice);
	VkPipelineShaderStageCreateInfo GetStageCreateInfo(KIND inKind) const;
protected:


private:
	VkShaderModule _CreateShaderModule(const std::vector<char>& code, VkDevice inDevice) const;

	VkShaderModule _vertShaderModule = nullptr;
	VkShaderModule _fragShaderModule = nullptr;

	fm::FilePath	_path;
};
}