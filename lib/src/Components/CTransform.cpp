#include "Components/CTransform.h"
#include "Entity.h"
#include <EntityManager.h>
#include "Core/Math/Matrix44.h"
#include "Core/Math/Functions.h"
//#include "TransformInspector.h"
using namespace fmc;
namespace Keys
{
const std::string position("position");
const std::string scale("scale");
const std::string rotation("rotation");
const std::string father("father");
const std::string layer("layer");
}
CTransform::CTransform()
{
    _name = "Transform";
	_idFather = Entity::INVALID;
	_hasFather = false;

	_position = { 0, 0, 0 };
	_scale = { 1, 1, 1 };
	_rotation = fm::math::Quaternion();
}

CTransform::CTransform(const fm::math::Vector3f& inPosition,
                       const fm::math::Vector3f& inScale,
                       const fm::math::Vector3f& inRotation)
{
	SetPosition(inPosition);
	SetScale(inScale);
	SetRotation(_rotation.FromEulerAngles(inRotation));

    _name = "Transform";
	_idFather = Entity::INVALID;
	_hasFather = false;
}

CTransform::~CTransform()
{
	
}

bool CTransform::Serialize(nlohmann::json &ioJson) const
{
    ioJson[Keys::position] = _position;
    ioJson[Keys::scale] = _scale;
    ioJson[Keys::rotation] = (fm::math::vec4)_rotation;
    ioJson[Keys::father] = _idFather.index();

    return true;
}

bool CTransform::Read(const nlohmann::json &inJSON)
{

	_position = inJSON[Keys::position];
	_scale = inJSON[Keys::scale];
	fm::math::vec4 q = inJSON[Keys::rotation];
	_rotation = fm::math::Quaternion(q);
	_idFather = EntityManager::get().CreateID(inJSON[Keys::father]);
	_isDirty = true;
	return true;
}



void CTransform::From(const fmc::CTransform *inTransform)
{
	_scale = inTransform->_scale;
	_position = inTransform->_position;
	_rotation = inTransform->_rotation;
	_idFather = inTransform->_idFather;
}




const std::string &CTransform::GetName() const
{
   return _name;
}

fm::Transform CTransform::GetTransform() const
{
	fm::Transform tr;
	tr.worldTransform = GetWorldPosMatrix();
	tr.worldRotation = GetWorldRotation();
	tr.position = _position;//TODO temporary fix
	tr.rotation = _rotation;
	tr.scale = _scale;
	return tr;
}


fm::math::mat CTransform::CreateMatrixModel(const fm::math::vec3& pos, const fm::math::vec3& scale, const fm::math::Quaternion& q, bool ortho)
{
	fm::math::mat model;
	model.identity();
	model = fm::math::translate(model, fm::math::vec3(pos.x, pos.y, pos.z));

	if (ortho)
	{
		model = fm::math::translate(model, fm::math::vec3(0.5f*scale.x, 0.5f * scale.y, 0.5f * scale.z));
		model = fm::math::rotate(model, q.GetRotationMatrix());
		model = fm::math::translate(model, fm::math::vec3(-0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z));
	}
	else
	{
		model = fm::math::rotate(model, q.GetRotationMatrix());
	}

	model = fm::math::scale(model, fm::math::vec3(scale.x, scale.y, scale.z));
	return model;
}


fm::math::mat CTransform::GetLocalMatrixModel(bool opposePosition) const
{
	fm::math::mat model;
	model.identity();
	if(opposePosition)
		model = fm::math::translate(model, fm::math::vec3(-_position.x, -_position.y, -_position.z));
	else
		model = fm::math::translate(model, fm::math::vec3(_position.x, _position.y, _position.z));

	model = fm::math::rotate(model, _rotation.GetRotationMatrix());
	
	model = fm::math::scale(model, fm::math::vec3(_scale.x, _scale.y, _scale.z));
	return model;
}


void CTransform::setFather(Entity* e)
{
    setFather(e->id());
}

void CTransform::RemoveFather()
{
	_hasFather = false;
	_isDirty = true;
	_idFather = Entity::INVALID;
}


void CTransform::setFather(Entity::Id id)
{
    _idFather = id;
	_hasFather = true;
	_isDirty = true;
}


void CTransform::SetRotation(const fm::math::Quaternion& inQuaternion)
{
	_isDirty = true;
	_rotation = inQuaternion;
}

void CTransform::SetScale(const fm::math::vec3& inScale)
{
	_isDirty = true;
	_scale = inScale;
}


void CTransform::SetPosition(const fm::math::vec3& inPosition)
{
	_isDirty = true;
	_position = inPosition;
}


const fm::math::mat& CTransform::GetWorldPosMatrix() const
{
	if (_isDirty)
	{
		_UpdateWorldMatrix();
	}
	return _worldPosition;
}


void CTransform::_UpdateWorldMatrix() const
{
	if (_isDirty)
	{
		const fm::math::mat m = GetLocalMatrixModel();

		if (_hasFather)
		{
			CTransform* father = GetFather();

			_worldPosition = father->GetWorldPosMatrix() * m;
			_worldRotation = father->GetWorldRotation() * _rotation;
		}
		else
		{
			_worldPosition = m;
			_worldRotation = _rotation;
		}


		_isDirty = false;
	}
}


fm::math::vec3 CTransform::GetWorldPosition() const
{
	if (_hasFather)
	{
		_UpdateWorldMatrix();
		return _worldPosition.Position();
	}

	return _position;
}




const fm::math::Quaternion& CTransform::GetWorldRotation() const
{
	if (_hasFather)
	{
		_UpdateWorldMatrix();
		return _worldRotation;
	}

	return _rotation;
}

fm::math::vec3 CTransform::GetWorldScale() const
{
	if (_hasFather)
	{
		_UpdateWorldMatrix();
		return _worldPosition.Scale();
	}

	return _scale;
}

void CTransform::SetWorldRotation(const fm::math::Quaternion& inQuaternion)
{
	SetRotation(_hasFather ? GetFather()->GetWorldRotation().Inverse() * inQuaternion : inQuaternion);
}

void CTransform::SetWorldScale(const fm::math::vec3& inScale)
{
	SetScale(_hasFather ? GetFather()->GetWorldPosMatrix().Scale() / inScale : inScale);
}


void CTransform::SetWorldPosition(const fm::math::vec3& inPosition)
{
	SetPosition(_hasFather ? fm::math::inverse(GetFather()->GetWorldPosMatrix()) * inPosition : inPosition);
}


const fm::math::vec3& CTransform::GetPosition() const
{
	return _position;
}

const fm::math::Quaternion& CTransform::GetRotation() const
{
	return _rotation;
}

const fm::math::vec3& CTransform::GetScale() const
{
	return _scale;
}

bool CTransform::HasFather() const
{
	return _hasFather;
}

Entity::Id	CTransform::GetFatherID() const
{
	return _idFather;
}


CTransform* CTransform::GetFather() const
{
	if (_hasFather)
	{
		Entity e = EntityManager::get().GetEntity(_idFather);
		return e.get<fmc::CTransform>();
	}

	return nullptr;
}