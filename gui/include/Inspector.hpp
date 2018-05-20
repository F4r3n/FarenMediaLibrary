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

    virtual void draw() {}
    virtual void setTarget(BaseComponent* compo) {compo = _component;}
    BaseComponent* _component = nullptr;
    static void OnDraw(fmc::CMesh *mesh, bool *value);
    static void OnDraw(fmc::CMaterial *material, bool *value);
    static void OnDraw(fmc::Body2D *body, bool *value);
    static void OnDraw(fmc::CDirectionalLight *dlight, bool *value);
    static void OnDraw(fmc::CTransform *transform, bool *value);
    static void OnDraw(fmc::CText *text, bool *value);
};
