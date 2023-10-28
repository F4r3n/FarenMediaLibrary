#pragma once

#include "Rendering/Shader.h"
#include "vulkan/vulkan.h"
#include "Core/Math/Matrix.h"

namespace fm
{
	class SubShader;
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

		VkShader(const fm::SubShader& inSubShader);

		~VkShader();

		bool Make(VkDevice inDevice);
		bool Delete(VkDevice inDevice);
		VkPipelineShaderStageCreateInfo GetStageCreateInfo(KIND inKind) const;
		fm::SubShader::Reflection GetReflection() { return _subShader->GetReflection(GRAPHIC_API::VULKAN); }
	protected:


	private:
		VkShaderModule _CreateShaderModule(const std::vector<char>& code, VkDevice inDevice) const;

		VkShaderModule _vertShaderModule = nullptr;
		VkShaderModule _fragShaderModule = nullptr;

		fm::FilePath	_path;
		std::optional<fm::SubShader> _subShader;

	};
}