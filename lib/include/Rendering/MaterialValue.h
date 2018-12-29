#pragma once
#include "Core/Math/Vector2.h"
#include "Core/Color.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Vector3.h"
#include <Core/Math/Vector.h>
#include <Rendering/Texture.h>
namespace fm {

struct TextureMat
{
        TextureMat(const TextureMat& textureMat)
        {
            texture = textureMat.texture;
            position = textureMat.position;
        }
        TextureMat() {position = 0;}

        fm::Texture texture;
        int position;
};


void to_json(nlohmann::json& j, const TextureMat& p);
void from_json(const nlohmann::json& j, TextureMat& p);


void to_json(nlohmann::json& j, const Texture& p);
void from_json(const nlohmann::json& j, Texture& p);

namespace MaterialValueNames
{
static const char* const ktypeName[] =
{
"Int", "Float", "Vector3", "Vector4", "Vector2",
"Matrix", "Texture", "Color", "None"
};
}

enum ValuesType
{
    VALUE_INT,
    VALUE_FLOAT,
    VALUE_VECTOR3_FLOAT,
    VALUE_VECTOR4_FLOAT,
    VALUE_VECTOR2_FLOAT,
    VALUE_MATRIX_FLOAT,
    VALUE_TEXTURE,
    VALUE_COLOR,
    VALUE_NONE,
    VALUE_LAST
    // etc
};

union VariantValue
{
    int int_;
    float float_;
    math::vec2 vec2_;

    math::vec3 vec3_;
    math::vec4 vec4_;
    math::mat mat_;
    TextureMat texture_;
    fm::Color color_;
        /// Construct uninitialized.
    VariantValue() { }
    /// Non-copyable.
    VariantValue(const VariantValue &value) = delete;
    /// Destruct.
    ~VariantValue() { }
};

class MaterialValue 
{
   public:
    MaterialValue() 
	{
        _valueType = ValuesType::VALUE_NONE;
    }

    MaterialValue(const MaterialValue& material) 
	{
        setType(material._valueType);
        setValue(material.getType(), material);
    }

    void setValue(fm::ValuesType valueType, const MaterialValue& material)
    {
        switch(valueType) {
        case ValuesType::VALUE_INT:
            _value.int_ = material._value.int_;
            break;
        case ValuesType::VALUE_FLOAT:
            _value.float_ = material._value.float_;
            break;
        case ValuesType::VALUE_VECTOR2_FLOAT:
            _value.vec2_ = material._value.vec2_;
            break;
        case ValuesType::VALUE_VECTOR3_FLOAT:
            _value.vec3_ = material._value.vec3_;
            break;
        case ValuesType::VALUE_VECTOR4_FLOAT:
            _value.vec4_ = material._value.vec4_;
            break;
        case ValuesType::VALUE_MATRIX_FLOAT:
            _value.mat_ = material._value.mat_;
            break;
        case ValuesType::VALUE_TEXTURE:
            _value.texture_ = material._value.texture_;
            break;
            case ValuesType::VALUE_COLOR:
			_value.color_ = material._value.color_;
			break;
        default:
            break;
        }
    }

    void setType(ValuesType valueType)
	{
        if(_valueType == valueType)
            return;

        _valueType = valueType;
        switch(valueType) 
		{
        case ValuesType::VALUE_VECTOR2_FLOAT:
            new (&_value.vec2_) math::vec2();
            break;
        case ValuesType::VALUE_VECTOR3_FLOAT:
            new (&_value.vec3_) math::vec3();
            break;
        case ValuesType::VALUE_VECTOR4_FLOAT:
            new (&_value.vec4_) math::vec4();
            break;
        case ValuesType::VALUE_MATRIX_FLOAT:
            new (&_value.mat_) math::mat();
            break;
            case ValuesType::VALUE_TEXTURE:
                new (&_value.texture_) fm::Texture();
                break;
            case ValuesType::VALUE_COLOR:
                new (&_value.color_) fm::Color();
                break;
        default:
            break;
        }
    }

    MaterialValue(int v)
	{
        setType(ValuesType::VALUE_INT);
        _value.int_ = static_cast<int>(v);
    }

    MaterialValue(float v) 
	{
        setType(ValuesType::VALUE_FLOAT);
        _value.float_ = static_cast<float>(v);
    }

    MaterialValue(math::vec2 v)
	{
        setType(ValuesType::VALUE_VECTOR2_FLOAT);
        _value.vec2_ = static_cast<math::vec2>(v);
    }

    MaterialValue(math::vec3 v)
	{
        setType(ValuesType::VALUE_VECTOR3_FLOAT);
        _value.vec3_ = static_cast<math::vec3>(v);
    }

    MaterialValue(math::vec4 v) 
	{
        setType(ValuesType::VALUE_VECTOR4_FLOAT);

        _value.vec4_ = static_cast<math::vec4>(v);
    }

    MaterialValue(math::mat v) 
	{
        setType(ValuesType::VALUE_MATRIX_FLOAT);

        _value.mat_ = static_cast<math::mat>(v);
    }

    MaterialValue(TextureMat v)
	{
        setType(ValuesType::VALUE_TEXTURE);

        _value.texture_ = static_cast<TextureMat>(v);
    }


    MaterialValue(fm::Color v) {
        setType(ValuesType::VALUE_COLOR);

        _value.color_ = static_cast<fm::Color>(v);
    }

    MaterialValue& operator=(MaterialValue & m)
    {
        setType(m.getType());
        setValue(m.getType(), m);
        return *this;
    }

    MaterialValue& operator=(int v)
	{
        setType(ValuesType::VALUE_INT);

        _value.int_ = (int)v;
        return *this;
    }

    MaterialValue& operator=(float v)
	{
        setType(ValuesType::VALUE_FLOAT);

        _value.float_ = (float)v;
        return *this;
    }

    MaterialValue& operator=(math::vec2 v) 
	{
        setType(ValuesType::VALUE_VECTOR2_FLOAT);

        _value.vec2_ = static_cast<math::vec2>(v);
        return *this;
    }

    MaterialValue& operator=(math::vec3 v)
	{
        setType(ValuesType::VALUE_VECTOR3_FLOAT);

        _value.vec3_ = static_cast<math::vec3>(v);
        return *this;
    }

    MaterialValue& operator=(math::vec4 v) 
	{
        setType(ValuesType::VALUE_VECTOR4_FLOAT);

        _value.vec4_ = static_cast<math::vec4>(v);
        return *this;
    }

    MaterialValue& operator=(math::mat v)
	{
        setType(ValuesType::VALUE_MATRIX_FLOAT);

        _value.mat_ = static_cast<math::mat>(v);
        return *this;
    }

    MaterialValue& operator=(TextureMat v)
	{
        setType(ValuesType::VALUE_TEXTURE);

        _value.texture_ = static_cast<TextureMat>(v);
        return *this;
    }

    MaterialValue& operator=(fm::Color v) 
	{
        setType(ValuesType::VALUE_COLOR);

        _value.color_ = static_cast<fm::Color>(v);
        return *this;
    }

    int getInt() const 
	{
        return _value.int_;
    }

    float getFloat() const
	{
        return _value.float_;
    }


    const math::vec2& getVector2() const
	{
        return _value.vec2_;
    }

    const math::vec3& getVector3() const 
	{
        return _value.vec3_;
    }

    const math::vec4& getVector4() const
	{
        return _value.vec4_;
    }

    const math::mat& getMatrix() const
	{
        return _value.mat_;
    }
    const TextureMat& getTexture() const
	{
        return _value.texture_;
    }

    const fm::Color getColor() const 
	{
        return _value.color_;
    }
    
    ValuesType getType() const
	{
        return _valueType;
    }

    template <typename T>
    const T& getAnyType() const
    {
        switch(_valueType) {
        case ValuesType::VALUE_INT:
            return _value.int_;
        case ValuesType::VALUE_FLOAT:
            return _value.float_;
        case ValuesType::VALUE_VECTOR2_FLOAT:
           return _value.vec2_;
        case ValuesType::VALUE_VECTOR3_FLOAT:
           return _value.vec3_;
        case ValuesType::VALUE_VECTOR4_FLOAT:
           return _value.vec4_;
        case ValuesType::VALUE_MATRIX_FLOAT:
           return _value.mat_;
        case ValuesType::VALUE_TEXTURE:
            return _value.texture_;
        case ValuesType::VALUE_COLOR:
            return _value.color_;

        }
        return 0;
    }

    //bool operator==(const MaterialValue &materialValue)
    //{
    //    return valueType == materialValue.valueType &&
    //}

   private:
    ValuesType _valueType = ValuesType::VALUE_NONE;
    VariantValue _value;
};
}
