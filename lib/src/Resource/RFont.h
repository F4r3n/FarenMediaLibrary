#pragma once
#include <unordered_map>

#include <Core/Config.h>
#include <Component.h>
#include "Core/Math/Vector2.h"
#include "Resource/Resource.h"

struct Character {
    fm::math::vec2 advance;
    fm::math::vec2 b_wh;
    fm::math::vec2 b_lt;
    fm::math::vec2 t;    
};

namespace fm
{
	class Texture;
}
namespace fm
{
	class RFont : public fm::Resource
	{
	public:
		RFont(const std::string& name);
		RFont();
		~RFont();
		static constexpr fm::RESOURCE_TYPE getType() { return fm::RESOURCE_TYPE::FONT; }
		std::unordered_map<GLchar, Character> Characters;

		fm::Texture* texture;
		//GLuint tex;
		int atlas_width;
		int atlas_height;
		void Reload(bool force = false) {}

	private:


	};
}