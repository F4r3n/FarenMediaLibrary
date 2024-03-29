#pragma once
#include "Core/Math/Vector2.h"
#include "Core/Color.h"
#include "Core/Math/Matrix44.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"
#include <Core/Math/Vector.h>

#include <variant>
#include "Core/FilePath.h"
namespace fm
{
	class Texture;
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
		VALUE_VECTOR4_UINTEGER,
		VALUE_LAST,
		VALUE_NONE
		// etc
	};
	using MaterialVariant = std::variant<int, float, fm::math::vec2, fm::math::vec3, fm::math::vec4, fm::math::vec4ui, fm::math::mat, fm::Color, fm::FilePath>;
	class MaterialValue
	{
	public:
		MaterialValue();

		MaterialValue(const MaterialValue& material);


		void setValue(fm::ValuesType valueType, const MaterialValue& material);

		void setValue(fm::ValuesType valueType);

		MaterialValue(int v);
		MaterialValue(float v);
		MaterialValue(math::vec2 v);
		MaterialValue(math::vec3 v);
		MaterialValue(math::vec4 v);
		MaterialValue(math::vec4ui v);

		MaterialValue(math::mat v);
		MaterialValue(std::shared_ptr<fm::Texture> v);
		MaterialValue(fm::Color v);

		MaterialValue& operator=(const MaterialValue& m);
		MaterialValue& operator=(int v);
		MaterialValue& operator=(float v);
		MaterialValue& operator=(math::vec2 v);
		MaterialValue& operator=(math::vec3 v);
		MaterialValue& operator=(math::vec4 v);
		MaterialValue& operator=(math::vec4ui v);

		MaterialValue& operator=(math::mat v);
		MaterialValue& operator=(std::shared_ptr<fm::Texture> v);
		MaterialValue& operator=(fm::Color v);

		int getInt() const;
		float getFloat() const;
		const math::vec2& getVector2() const;
		const math::vec3& getVector3() const;
		const math::vec4& getVector4() const;
		const math::vec4ui& getVector4ui() const;

		const math::mat& getMatrix() const;
		std::shared_ptr<fm::Texture> getTexture() const;
		const fm::Color getColor() const;

		ValuesType getType() const;

		template <typename T>
		const T& getAnyType() const;

		MaterialVariant GetConstVariant() const;
		MaterialVariant& GetVariant();

	private:
		ValuesType _valueType;
		MaterialVariant	_value;
	};
}
