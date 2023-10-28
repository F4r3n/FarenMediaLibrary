#pragma once
#include "Core/Math/Vector2.h"
#include "Core/Color.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Vector3.h"
#include <Core/Math/Vector.h>
#include "Rendering/Texture.h"
#include <variant>
namespace fm {

	struct TextureMat
	{
		fm::Texture texture;
		int position;
	};


	void to_json(nlohmann::json& j, const TextureMat& p);
	void from_json(const nlohmann::json& j, TextureMat& p);


	void to_json(nlohmann::json& j, const Texture& p);
	void from_json(const nlohmann::json& j, Texture& p);

	enum class ValuesType
	{
		VALUE_INT,
		VALUE_FLOAT,
		VALUE_VECTOR3_FLOAT,
		VALUE_VECTOR4_FLOAT,
		VALUE_VECTOR2_FLOAT,
		VALUE_MATRIX_FLOAT,
		VALUE_TEXTURE,
		VALUE_COLOR,
		VALUE_LAST,
		VALUE_NONE
		// etc
	};
	using MaterialVariant = std::variant<int, float, fm::math::vec2, fm::math::vec3, fm::math::vec4, fm::math::mat, fm::Color, TextureMat>;
	class MaterialValue
	{
	public:
		MaterialValue()
			:_valueType(ValuesType::VALUE_NONE)
		{
		}

		MaterialValue(const MaterialValue& material)
		{
			setValue(material.getType(), material);
		}


		void setValue(fm::ValuesType valueType, const MaterialValue& material)
		{
			_valueType = valueType;
			_value = material._value;
		}

		void setValue(fm::ValuesType valueType)
		{
			_valueType = valueType;
			switch (_valueType)
			{
			case fm::ValuesType::VALUE_INT:
				_value = 0;
				break;
			case fm::ValuesType::VALUE_FLOAT:
				_value = 0.0f;
				break;
			case fm::ValuesType::VALUE_VECTOR3_FLOAT:
				_value = fm::math::vec3(0, 0, 0);
				break;
			case fm::ValuesType::VALUE_VECTOR4_FLOAT:
				_value = fm::math::vec4(0, 0, 0, 0);
				break;
			case fm::ValuesType::VALUE_VECTOR2_FLOAT:
				_value = fm::math::vec2(0, 0);
				break;
			case fm::ValuesType::VALUE_MATRIX_FLOAT:
			{
				fm::math::mat mat;
				mat.identity();
				_value = mat;
			}
			break;
			case fm::ValuesType::VALUE_TEXTURE:
			{
				fm::TextureMat mat;
				_value = mat;
			}
			break;
			case fm::ValuesType::VALUE_COLOR:
				_value = fm::Color(0.0f, 0.0f, 0.0f, 1.0f);
				break;
			default:
				break;
			}
		}

		MaterialValue(int v)
		{
			_valueType = (ValuesType::VALUE_INT);
			_value = v;
		}

		MaterialValue(float v)
		{
			_valueType = (ValuesType::VALUE_FLOAT);
			_value = v;
		}

		MaterialValue(math::vec2 v)
		{
			_valueType = (ValuesType::VALUE_VECTOR2_FLOAT);
			_value = v;
		}

		MaterialValue(math::vec3 v)
		{
			_valueType = (ValuesType::VALUE_VECTOR3_FLOAT);
			_value = v;
		}

		MaterialValue(math::vec4 v)
		{
			_valueType = (ValuesType::VALUE_VECTOR4_FLOAT);
			_value = v;
		}

		MaterialValue(math::mat v)
		{
			_valueType = (ValuesType::VALUE_MATRIX_FLOAT);
			_value = v;
		}

		MaterialValue(TextureMat v)
		{
			_valueType = (ValuesType::VALUE_TEXTURE);
			_value = v;
		}


		MaterialValue(fm::Color v)
		{
			_valueType = (ValuesType::VALUE_COLOR);
			_value = v;
		}

		MaterialValue& operator=(const MaterialValue& m)
		{
			setValue(m.getType(), m);
			return *this;
		}

		MaterialValue& operator=(int v)
		{
			_valueType = (ValuesType::VALUE_INT);
			_value = v;
			return *this;
		}

		MaterialValue& operator=(float v)
		{
			_valueType = (ValuesType::VALUE_FLOAT);
			_value = (float)v;
			return *this;
		}

		MaterialValue& operator=(math::vec2 v)
		{
			_valueType = (ValuesType::VALUE_VECTOR2_FLOAT);
			_value = static_cast<math::vec2>(v);
			return *this;
		}

		MaterialValue& operator=(math::vec3 v)
		{
			_valueType = (ValuesType::VALUE_VECTOR3_FLOAT);
			_value = static_cast<math::vec3>(v);
			return *this;
		}

		MaterialValue& operator=(math::vec4 v)
		{
			_valueType = (ValuesType::VALUE_VECTOR4_FLOAT);
			_value = static_cast<math::vec4>(v);
			return *this;
		}

		MaterialValue& operator=(math::mat v)
		{
			_valueType = (ValuesType::VALUE_MATRIX_FLOAT);
			_value = static_cast<math::mat>(v);
			return *this;
		}

		MaterialValue& operator=(TextureMat v)
		{
			_valueType = (ValuesType::VALUE_TEXTURE);
			_value = static_cast<TextureMat>(v);
			return *this;
		}

		MaterialValue& operator=(fm::Color v)
		{
			_valueType = (ValuesType::VALUE_COLOR);
			_value = static_cast<fm::Color>(v);
			return *this;
		}

		int getInt() const
		{
			return std::get<int>(_value);
		}

		float getFloat() const
		{
			return std::get<float>(_value);
		}


		const math::vec2& getVector2() const
		{
			return std::get<math::vec2>(_value);
		}

		const math::vec3& getVector3() const
		{
			return std::get<fm::math::vec3>(_value);
		}

		const math::vec4& getVector4() const
		{
			return std::get<fm::math::vec4>(_value);
		}

		const math::mat& getMatrix() const
		{
			return std::get<fm::math::mat>(_value);
		}
		const TextureMat& getTexture() const
		{
			return std::get<TextureMat>(_value);
		}

		const fm::Color getColor() const
		{
			return std::get<fm::Color>(_value);
		}

		ValuesType getType() const
		{
			return _valueType;
		}

		template <typename T>
		const T& getAnyType() const
		{
			return std::get<T>(_value);
		}

		MaterialVariant& GetVariant() { return _value; }
	private:
		ValuesType _valueType;
		MaterialVariant	_value;
	};
}
