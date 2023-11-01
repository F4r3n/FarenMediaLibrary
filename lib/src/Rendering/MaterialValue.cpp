#include "MaterialValue.h"
#include <nlohmann/json.hpp>
namespace fm {


	MaterialValue::MaterialValue()
		:_valueType(ValuesType::VALUE_NONE)
	{
	}

	MaterialValue::MaterialValue(const MaterialValue& material)
	{
		setValue(material.getType(), material);
	}


	void MaterialValue::setValue(fm::ValuesType valueType, const MaterialValue& material)
	{
		_valueType = valueType;
		_value = material._value;
	}

	void MaterialValue::setValue(fm::ValuesType valueType)
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

	MaterialValue::MaterialValue(int v)
	{
		_valueType = (ValuesType::VALUE_INT);
		_value = v;
	}

	MaterialValue::MaterialValue(float v)
	{
		_valueType = (ValuesType::VALUE_FLOAT);
		_value = v;
	}

	MaterialValue::MaterialValue(math::vec2 v)
	{
		_valueType = (ValuesType::VALUE_VECTOR2_FLOAT);
		_value = v;
	}

	MaterialValue::MaterialValue(math::vec3 v)
	{
		_valueType = (ValuesType::VALUE_VECTOR3_FLOAT);
		_value = v;
	}

	MaterialValue::MaterialValue(math::vec4 v)
	{
		_valueType = (ValuesType::VALUE_VECTOR4_FLOAT);
		_value = v;
	}

	MaterialValue::MaterialValue(math::mat v)
	{
		_valueType = (ValuesType::VALUE_MATRIX_FLOAT);
		_value = v;
	}

	MaterialValue::MaterialValue(TextureMat v)
	{
		_valueType = (ValuesType::VALUE_TEXTURE);
		_value = v;
	}


	MaterialValue::MaterialValue(fm::Color v)
	{
		_valueType = (ValuesType::VALUE_COLOR);
		_value = v;
	}

	MaterialValue& MaterialValue::operator=(const MaterialValue& m)
	{
		setValue(m.getType(), m);
		return *this;
	}

	MaterialValue& MaterialValue::operator=(int v)
	{
		_valueType = (ValuesType::VALUE_INT);
		_value = v;
		return *this;
	}

	MaterialValue& MaterialValue::operator=(float v)
	{
		_valueType = (ValuesType::VALUE_FLOAT);
		_value = (float)v;
		return *this;
	}

	MaterialValue& MaterialValue::operator=(math::vec2 v)
	{
		_valueType = (ValuesType::VALUE_VECTOR2_FLOAT);
		_value = static_cast<math::vec2>(v);
		return *this;
	}

	MaterialValue& MaterialValue::operator=(math::vec3 v)
	{
		_valueType = (ValuesType::VALUE_VECTOR3_FLOAT);
		_value = static_cast<math::vec3>(v);
		return *this;
	}

	MaterialValue& MaterialValue::operator=(math::vec4 v)
	{
		_valueType = (ValuesType::VALUE_VECTOR4_FLOAT);
		_value = static_cast<math::vec4>(v);
		return *this;
	}

	MaterialValue& MaterialValue::operator=(math::mat v)
	{
		_valueType = (ValuesType::VALUE_MATRIX_FLOAT);
		_value = static_cast<math::mat>(v);
		return *this;
	}

	MaterialValue& MaterialValue::operator=(TextureMat v)
	{
		_valueType = (ValuesType::VALUE_TEXTURE);
		_value = static_cast<TextureMat>(v);
		return *this;
	}

	MaterialValue& MaterialValue::operator=(fm::Color v)
	{
		_valueType = (ValuesType::VALUE_COLOR);
		_value = static_cast<fm::Color>(v);
		return *this;
	}

	int MaterialValue::getInt() const
	{
		return std::get<int>(_value);
	}

	float MaterialValue::getFloat() const
	{
		return std::get<float>(_value);
	}


	const math::vec2& MaterialValue::getVector2() const
	{
		return std::get<math::vec2>(_value);
	}

	const math::vec3& MaterialValue::getVector3() const
	{
		return std::get<fm::math::vec3>(_value);
	}

	const math::vec4& MaterialValue::getVector4() const
	{
		return std::get<fm::math::vec4>(_value);
	}

	const math::mat& MaterialValue::getMatrix() const
	{
		return std::get<fm::math::mat>(_value);
	}
	const TextureMat& MaterialValue::getTexture() const
	{
		return std::get<TextureMat>(_value);
	}

	const fm::Color MaterialValue::getColor() const
	{
		return std::get<fm::Color>(_value);
	}

	ValuesType MaterialValue::getType() const
	{
		return _valueType;
	}

	template <typename T>
	const T& MaterialValue::getAnyType() const
	{
		return std::get<T>(_value);
	}

	MaterialVariant& MaterialValue::GetVariant() { return _value; }
}
