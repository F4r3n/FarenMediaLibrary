#pragma once

#include "component.h"
#include "Core/Math/Vector2.h"


namespace fm
{
	class RFont;
	struct Transform;
namespace rendering
{
class VertexBuffer;
}
}

class EntityManager;
namespace fmc
{

class CText
{

    public:

		enum class TEXT_RENDERING
		{
			OVERLAY,
			WORLD_SPACE
		};

        CText(const std::string& text, const std::string& fontName);
        CText();
        ~CText();

		bool							Serialize(nlohmann::json& ioJson) const;
		bool							Read(const nlohmann::json& inJSON);
		uint16_t						GetType() const { return kText; }

		void							SetText(const std::string& inText);
		const std::string&				GetText() const;
		void							UpdateBuffer(const fm::Transform& inTransform, fm::RFont *inFont);
		fm::rendering::VertexBuffer*	GetVertexBuffer() const;
		TEXT_RENDERING					GetTextType() const { return _rendering; }
		const std::string&				GetFontName() const { return _fontName; }
		std::string						GetName() const { return "Text"; }

    private:
		TEXT_RENDERING									_rendering = TEXT_RENDERING::OVERLAY;
		std::unique_ptr<fm::rendering::VertexBuffer>	_buffer;
		std::string										_fontName = "dejavu";
		std::string										_text = "";
		bool											_isDirty = false;
private:
	std::string _name;

};
}
