#include "Components/CText.h"
#include <EntityManager.h>
#include "Core/Math/Vector4.h"
#include "Rendering/VertexBuffer.hpp"
#include "Resource/RFont.h"
#include "Core/Transform.h"
#include "nlohmann/json.hpp"
#include "Core/Debug.h"
using namespace fmc;

CText::CText()
{
     _name = "Text";
	 _buffer = nullptr;
}

CText::CText(const std::string& text, const std::string& fontName)
{
    _text = text;
    _fontName = fontName;
	_isDirty = true;
    _name = "Text";
	_buffer = nullptr;
}

void CText::SetText(const std::string& inText)
{
	if (_text != inText)
	{
		_isDirty = true;
		_text = inText;
	}
}

fm::rendering::VertexBuffer* CText::GetVertexBuffer() const
{
	return _buffer.get();
}


void CText::UpdateBuffer(const fm::Transform& inTransform, fm::RFont* inFont)
{
	fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);

	if (_buffer == nullptr)
	{
		_buffer = std::make_unique<fm::rendering::VertexBuffer>();
		_buffer->generate();
		_buffer->SetVertexAttribArray(0, 4, fm::rendering::VertexBuffer::DATA_TYPE::FLOAT, 4 * sizeof(float));
	}
	fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);


	if (_isDirty)
	{
		fm::math::vec3 worldSpace = inTransform.position;
		fm::math::vec4* coords = new fm::math::vec4[6 * _text.size()];
		float x = 0;
		float y = 0;
		std::string::const_iterator c;
		int n = 0;
		for (c = _text.begin(); c != _text.end(); c++)
		{
			Character ch = inFont->Characters[*c];
			float scaleY = 1;
			float scaleX = 1;
			float x2 = x + ch.b_lt.x * scaleX;
			float y2 = -y - ch.b_lt.y * scaleY;
			float w = ch.b_wh.x * scaleX;
			float h = ch.b_wh.y * scaleY;
			x += ch.advance.x * scaleX;
			y += ch.advance.y * scaleY;
			if (!w || !h)
				continue;

			coords[n++] = fm::math::vec4(x2,		-y2,		ch.t.x, ch.t.y);
			coords[n++] = fm::math::vec4(x2 + w,	-y2,		ch.t.x + ch.b_wh.x / inFont->atlas_width, ch.t.y);
			coords[n++] = fm::math::vec4(x2,		-y2 - h,	ch.t.x, ch.t.y + ch.b_wh.y / inFont->atlas_height);
			coords[n++] = fm::math::vec4(x2 + w,	-y2,		ch.t.x + ch.b_wh.x / inFont->atlas_width, ch.t.y);
			coords[n++] = fm::math::vec4(x2,		-y2 - h,	ch.t.x, ch.t.y + ch.b_wh.y / inFont->atlas_height);
			coords[n++] = fm::math::vec4(x2 + w,	-y2 - h,	ch.t.x + ch.b_wh.x / inFont->atlas_width, ch.t.y + ch.b_wh.y / inFont->atlas_height);
		}
		_buffer->setBufferData(&coords[0], 0, n, sizeof(float) * 4, false);
		delete [] coords;
	}
	fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);


	_isDirty = false;
}


const std::string& CText::GetText() const
{
	return _text;
}

void CText::Destroy()
{
    EntityManager::get().removeComponent<CText>(BaseComponent::_IDEntity);
}

CText::~CText()
{
}

bool CText::Serialize(nlohmann::json& ioJson) const
{
	ioJson["text"] = _text;
	ioJson["fontName"] = _fontName;
	ioJson["rendering"] = (int)_rendering;
	return true;
}

bool CText::Read(const nlohmann::json& inJSON)
{
	_text = inJSON["text"];
	_fontName = inJSON["fontName"];
	_rendering = (fmc::CText::TEXT_RENDERING)inJSON["rendering"];
	return true;
}
