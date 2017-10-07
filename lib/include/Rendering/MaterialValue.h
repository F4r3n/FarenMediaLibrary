#pragma once
#include "Core/Math/Vector2.h"
#include "Core/Color.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Vector3.h"
#include <Core/Math/Vector.h>
namespace fm {

enum ValuesType {
    VALUE_INT,
    VALUE_FLOAT,
    VALUE_VECTOR3_FLOAT,
    VALUE_VECTOR4_FLOAT,
    VALUE_VECTOR2_FLOAT,
    VALUE_MATRIX_FLOAT,
    VALUE_NONE
    // etc
};

union VariantValue {
    int int_;
    float float_;
    math::vec2 vec2_;

    math::vec3 vec3_;
    math::vec4 vec4_;
    math::mat mat_;
    
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
    
    ValuesType getType() const{
        return valueType;
    }

   private:
    ValuesType valueType = ValuesType::VALUE_NONE;
    VariantValue value;
};
}