#pragma once
#include <unordered_map>

#include <Core/Config.h>
#include <Component.h>
#include "Core/Math/Vector2.h"
#include "Resource/Resource.h"

struct Character {
    fm::math::vec2l advance;
    fm::math::vec2l b_wh;
    fm::math::vec2l b_lt;
    fm::math::vec2 t;    
};

namespace fm
{
	class OGLTexture;
}
namespace fm
{
	class RFont : public fm::Resource
	{
	public:
		RFont(const fm::FilePath& inPath);
		RFont();
		~RFont();
		static constexpr fm::RESOURCE_TYPE getType() { return fm::RESOURCE_TYPE::FONT; }
		virtual fm::RESOURCE_TYPE GetType() const override { return getType(); }

		std::unordered_map<char, Character> Characters;

		fm::OGLTexture* texture;
		//GLuint tex;
		int atlas_width;
		int atlas_height;
		void Reload(bool force = false) {}

	private:


	};
}