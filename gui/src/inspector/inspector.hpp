#pragma once

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

    virtual void Draw(bool *) {}
    virtual void SetTarget(BaseComponent* compo) {compo = _component;}
    BaseComponent* _component = nullptr;

};
