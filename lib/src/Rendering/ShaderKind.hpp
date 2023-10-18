#pragma once
#include <cstdint>
namespace fm
{
	enum SHADER_KIND : uint32_t
	{
		PLAIN = 0,
		TEXTURE = 1
	};

	using ShaderID = uint64_t;

}
