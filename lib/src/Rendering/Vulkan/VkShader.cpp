#include "VkShader.hpp"
#include "Core/Debug.h"
using namespace fm;

VkShader::VkShader()
{

}

VkShader::VkShader(const fm::FilePath& inFilePath, const std::string& name) : Shader(inFilePath)
{

}

VkShader::~VkShader()
{

}


bool VkShader::compile()
{
	return false;
}

VkShaderModule VkShader::_CreateShaderModule(const std::vector<char>& code, VkDevice inDevice) const
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(inDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}
	return shaderModule;
}



bool VkShader::Make(VkDevice inDevice)
{
	if (_vertShaderModule != nullptr && _fragShaderModule != nullptr)
		return true;

	_path.ToSub("SPIR-V");
	File frag(Folder(_path), "frag.spv");
	File vert(Folder(_path), "vert.spv");

	if (!frag.Exist())
	{
		fm::Debug::get().LogError(frag.GetPath().GetPath().string() + " Frag is not compiled");
		return false;
	}

	if (!vert.Exist())
	{
		fm::Debug::get().LogError(vert.GetPath().GetPath().string() + " Vert is not compiled");
		return false;
	}

	auto vertCode = vert.GetBinaryContent();
	auto fragCode = frag.GetBinaryContent();

	if (!(vertCode.has_value() && fragCode.has_value()))
		return false;

	_vertShaderModule = _CreateShaderModule(*vertCode, inDevice);
	_fragShaderModule = _CreateShaderModule(*fragCode, inDevice);

	return true;
}

VkPipelineShaderStageCreateInfo VkShader::GetStageCreateInfo(KIND inKind) const
{
	VkPipelineShaderStageCreateInfo shaderStageInfo{};

	if (inKind == KIND::VERT)
	{
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStageInfo.module = _vertShaderModule;
		shaderStageInfo.pName = "main";
	}
	else if (inKind == KIND::FRAG)
	{
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStageInfo.module = _fragShaderModule;
		shaderStageInfo.pName = "main";
	}
	return shaderStageInfo;
}


bool VkShader::Delete(VkDevice inDevice)
{
	vkDestroyShaderModule(inDevice, _fragShaderModule, nullptr);
	vkDestroyShaderModule(inDevice, _vertShaderModule, nullptr);
	return true;
}
