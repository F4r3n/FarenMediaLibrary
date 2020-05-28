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
namespace fmc {

class CText : public FMComponent<CText>
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

		bool							Serialize(nlohmann::json& ioJson) const override;
		bool							Read(const nlohmann::json& inJSON) override;
		uint16_t						GetType() const override { return kText; }

        void							Destroy() override;
		void							SetText(const std::string& inText);
		const std::string&				GetText() const;
		void							UpdateBuffer(const fm::Transform& inTransform, fm::RFont *inFont);
		fm::rendering::VertexBuffer*	GetVertexBuffer() const { return _buffer.get(); }
		TEXT_RENDERING					GetTextType() const { return _rendering; }
		const std::string&				GetFontName() const { return _fontName; }

    private:
		TEXT_RENDERING									_rendering = TEXT_RENDERING::OVERLAY;
		std::unique_ptr<fm::rendering::VertexBuffer>	_buffer = nullptr;
		std::string										_fontName = "dejavu";
		std::string										_text = "";
		bool											_isDirty = false;

};
}
