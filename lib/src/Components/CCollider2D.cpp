#include "Components/CCollider2D.h"
#include "EntityManager.h"
#include "Core/Transform.h"
#include <Box2D/Box2D.h>
#include "Physic/PhysicSystem.h"
#include <nlohmann/json.hpp>
using namespace fmc;

CCollider2D::CCollider2D()
{
	_collisionShape = nullptr;
	_scale = fm::math::vec2(1, 1);
	_name = "Collider";
	_shape = CCollider2D::SHAPE::RECTANGLE;
}

CCollider2D::~CCollider2D()
{
	delete _collisionShape;
}

void CCollider2D::Init(const fm::Transform& inTransform)
{
	if (_collisionShape == nullptr)
	{

		switch (_shape)
		{
		case fmc::CCollider2D::SHAPE::RECTANGLE:
		{
			b2PolygonShape* s = new b2PolygonShape();
			s->SetAsBox(_scale.x * fms::PhysicSystem2D::P2M(), _scale.y * fms::PhysicSystem2D::P2M());
			_collisionShape = s;
		}
			break;
		case fmc::CCollider2D::SHAPE::DISK:
		{
			b2CircleShape* s = new b2CircleShape();
			s->m_radius = _scale.x * fms::PhysicSystem2D::P2M();
			_collisionShape = s;
		}
			break;
		default:
			break;
		}
	}
}

bool CCollider2D::IsInit()
{
	return _collisionShape != nullptr;
}


bool CCollider2D::Serialize(nlohmann::json &ioJson) const
{
	ioJson["scale"] = _scale;
	ioJson["shape"] = _shape;

	return true;
}
bool CCollider2D::Read(const nlohmann::json &inJSON)
{
	_scale = inJSON["scale"];
	_shape = inJSON["shape"];
	return true;
}

bool CCollider2D::SetShape(SHAPE inShape)
{
	if (!IsInit())
	{
		_shape = inShape;
		return true;
	}
	return false;
}

CCollider2D::SHAPE CCollider2D::GetShape() const
{
	return _shape;
}


const std::string& CCollider2D::GetName() const
{
	return _name;
}


void CCollider2D::Destroy()
{
	EntityManager::get().removeComponent<fmc::CCollider2D>(BaseComponent::_IDEntity);
}


const fm::math::vec2  CCollider2D::GetScale() const
{
	return _scale;
}


void CCollider2D::SetScale(const fm::math::vec2 &inScale)
{
	_scale = inScale;//The component should be recreated
}