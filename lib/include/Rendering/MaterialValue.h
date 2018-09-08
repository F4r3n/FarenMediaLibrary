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

class MaterialValue {
   public:
    MaterialValue() {
        valueType = ValuesType::VALUE_NONE;
    }

    MaterialValue(const MaterialValue& material) {
        setType(material.valueType);
        setValue(material.getType(), material);
    }

    void setValue(fm::ValuesType valueType, const MaterialValue& material)
    {
        switch(valueType) {
        case ValuesType::VALUE_INT:
            value.int_ = material.value.int_;
            break;
        case ValuesType::VALUE_FLOAT:
            value.float_ = material.value.float_;
            break;
        case ValuesType::VALUE_VECTOR2_FLOAT:
            value.vec2_ = material.value.vec2_;
            break;
        case ValuesType::VALUE_VECTOR3_FLOAT:
            value.vec3_ = material.value.vec3_;
            break;
        case ValuesType::VALUE_VECTOR4_FLOAT:
            value.vec4_ = material.value.vec4_;
            break;
        case ValuesType::VALUE_MATRIX_FLOAT:
            value.mat_ = material.value.mat_;
            break;
        case ValuesType::VALUE_TEXTURE:
            value.texture_ = material.value.texture_;
            break;
            case ValuesType::VALUE_COLOR:
                value.color_ = material.value.color_;
                break;
        default:
            break;
        }
    }

    void setType(ValuesType valueType) {
        if(this->valueType == valueType)
            return;

        this->valueType = valueType;
        switch(valueType) {
        case ValuesType::VALUE_VECTOR2_FLOAT:
            new (&value.vec2_) math::vec2();
            break;
        case ValuesType::VALUE_VECTOR3_FLOAT:
            new (&value.vec3_) math::vec3();
            break;
        case ValuesType::VALUE_VECTOR4_FLOAT:
            new (&value.vec4_) math::vec4();
            break;
        case ValuesType::VALUE_MATRIX_FLOAT:
            new (&value.mat_) math::mat();
            break;
            case ValuesType::VALUE_TEXTURE:
                new (&value.texture_) fm::Texture();
                break;
            case ValuesType::VALUE_COLOR:
                new (&value.color_) fm::Color();
                break;
        default:
            break;
        }
    }

    MaterialValue(int v) {
        setType(ValuesType::VALUE_INT);
        value.int_ = static_cast<int>(v);
    }

    MaterialValue(float v) {
        setType(ValuesType::VALUE_FLOAT);
        value.float_ = static_cast<float>(v);
    }

    MaterialValue(math::vec2 v) {
        setType(ValuesType::VALUE_VECTOR2_FLOAT);
        value.vec2_ = static_cast<math::vec2>(v);
    }

    MaterialValue(math::vec3 v) {
        setType(ValuesType::VALUE_VECTOR3_FLOAT);
        value.vec3_ = static_cast<math::vec3>(v);
    }

    MaterialValue(math::vec4 v) {
        setType(ValuesType::VALUE_VECTOR4_FLOAT);

        value.vec4_ = static_cast<math::vec4>(v);
    }

    MaterialValue(math::mat v) {
        setType(ValuesType::VALUE_MATRIX_FLOAT);

        value.mat_ = static_cast<math::mat>(v);
    }

    MaterialValue(TextureMat v) {
        setType(ValuesType::VALUE_TEXTURE);

        value.texture_ = static_cast<TextureMat>(v);
    }

    MaterialValue(fm::Color v) {
        setType(ValuesType::VALUE_COLOR);

        value.color_ = static_cast<fm::Color>(v);
    }

    MaterialValue& operator=(MaterialValue & m)
    {
        setType(m.getType());
        setValue(m.getType(), m);
        return *this;
    }

    MaterialValue& operator=(int v) {
        setType(ValuesType::VALUE_INT);

        value.int_ = (int)v;
        return *this;
    }

    MaterialValue& operator=(float v) {
        setType(ValuesType::VALUE_FLOAT);

        value.float_ = (float)v;
        return *this;
    }

    MaterialValue& operator=(math::vec2 v) {
        setType(ValuesType::VALUE_VECTOR2_FLOAT);

        value.vec2_ = static_cast<math::vec2>(v);
        return *this;
    }

    MaterialValue& operator=(math::vec3 v) {
        setType(ValuesType::VALUE_VECTOR3_FLOAT);

        value.vec3_ = static_cast<math::vec3>(v);
        return *this;
    }

    MaterialValue& operator=(math::vec4 v) {
        setType(ValuesType::VALUE_VECTOR4_FLOAT);

        value.vec4_ = static_cast<math::vec4>(v);
        return *this;
    }

    MaterialValue& operator=(math::mat v) {
        setType(ValuesType::VALUE_MATRIX_FLOAT);

        value.mat_ = static_cast<math::mat>(v);
        return *this;
    }

    MaterialValue& operator=(TextureMat v) {
        setType(ValuesType::VALUE_TEXTURE);

        value.texture_ = static_cast<TextureMat>(v);
        return *this;
    }

    MaterialValue& operator=(fm::Color v) {
        setType(ValuesType::VALUE_COLOR);

        value.color_ = static_cast<fm::Color>(v);
        return *this;
    }

    int getInt() const {
        return value.int_;
    }

    float getFloat() const {
        return value.float_;
    }


    const math::vec2& getVector2() const {
        return value.vec2_;
    }

    const math::vec3& getVector3() const {
        return value.vec3_;
    }

    const math::vec4& getVector4() const {
        return value.vec4_;
    }

    const math::mat& getMatrix() const {
        return value.mat_;
    }
    const TextureMat& getTexture() const {
        return value.texture_;
    }

    const fm::Color getColor() const {
        return value.color_;
    }
    
    ValuesType getType() const{
        return valueType;
    }

    template <typename T>
    const T& getAnyType() const
    {
        switch(valueType) {
        case ValuesType::VALUE_INT:
            return value.int_;
        case ValuesType::VALUE_FLOAT:
            return value.float_;
        case ValuesType::VALUE_VECTOR2_FLOAT:
           return value.vec2_;
        case ValuesType::VALUE_VECTOR3_FLOAT:
           return value.vec3_;
        case ValuesType::VALUE_VECTOR4_FLOAT:
           return value.vec4_;
        case ValuesType::VALUE_MATRIX_FLOAT:
           return value.mat_;
        case ValuesType::VALUE_TEXTURE:
            return value.texture_;
        case ValuesType::VALUE_COLOR:
            return value.color_;

        }
        return 0;
    }

    //bool operator==(const MaterialValue &materialValue)
    //{
    //    return valueType == materialValue.valueType &&
    //}

   private:
    ValuesType valueType = ValuesType::VALUE_NONE;
    VariantValue value;
};
}
