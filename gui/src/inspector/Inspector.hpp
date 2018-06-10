#pragma once
#include <ECS.h>
#include "Components/component.h"
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

    virtual void draw(bool *value) {}
    virtual void setTarget(BaseComponent* compo) {compo = _component;}
    BaseComponent* _component = nullptr;
    static void OnDraw(fmc::CDirectionalLight *dlight, bool *value);

};
