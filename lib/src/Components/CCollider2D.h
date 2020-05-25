#pragma once
#include "component.h"
#include "Core/Math/Vector2.h"

class b2PolygonShape;
class b2Shape;
namespace fm
{
	struct Transform;
}

namespace fmc
{

	class CCollider2D : public FMComponent<CCollider2D>
	{

	public:
		enum class SHAPE
		{
			RECTANGLE,
			DISK
		};

		CCollider2D();
		~CCollider2D();
		bool Serialize(nlohmann::json &ioJson) const override;
		bool Read(const nlohmann::json &inJSON) override;
		const std::string& GetName() const override;
		void Destroy() override;
		uint16_t GetType() const override { return kCollider2D; }

		void Init(const fm::Transform& inTransform);
		bool IsInit();

		b2Shape* GetCollisionShape() const { return _collisionShape; }
		bool SetShape(SHAPE inShape);
		SHAPE GetShape() const;

		const fm::math::vec2  GetScale() const;
		void SetScale(const fm::math::vec2 &inScale);
	private:
		SHAPE			_shape;
		fm::math::vec2	_scale;

		b2Shape*_collisionShape;
		//TODO Position/Rotation relative to the entity

	};
}