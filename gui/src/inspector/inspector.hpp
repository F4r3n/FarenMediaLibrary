#pragma once
#include "Entity.h"
class BaseComponent;
namespace fmc {
class CTransform;
class CMaterial;
class Body2D;
class CDirectionalLight;
class CMesh;
class CText;
}

class Inspector {
   public:
    Inspector();
    virtual ~Inspector();

    virtual void Draw(bool *, const Entity& inEntity) {}
    virtual void SetTarget(BaseComponent* compo) {compo = _component;}
	virtual void RemoveComponent(const Entity& e);
    BaseComponent* _component = nullptr;

};
