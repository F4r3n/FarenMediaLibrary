#include "VkShader.h"
#include "Core/Debug.h"
using namespace fm;

VkShader::VkShader()
{

}

VkShader::VkShader(const fm::FilePath& inFilePath, const std::string& name) : Shader(inFilePath, name)
{

}


bool VkShader::compile()
{

}

VkShaderModule VkShader::_CreateShaderModule(const std::vector<char>& code) const
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}
}



bool VkShader::_Load()
{
	File frag(Folder(_path.ToSubFolder("SPIR-V")), "frag.spv");
	File vert(Folder(_path.ToSubFolder("SPIR-V")), "vert.spv");

	if (!frag.Exist())
	{
		fm::Debug::get().LogError(frag.GetPath().GetPath() + " Frag is not compiled");
		return false;
	}

	if (!vert.Exist())
	{
		fm::Debug::get().LogError(vert.GetPath().GetPath() + " Vert is not compiled");
		return false;
	}

	auto vertCode = vert.GetBinaryContent();
	auto fragCode = frag.GetBinaryContent();



}