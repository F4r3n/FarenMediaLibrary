#pragma once
#include "Component.h"
#include "Rect.h"
namespace fmc {
class CCollider : public Component<CCollider> {
public:
    CCollider() {}
    ~CCollider() {}
    size_t idCollision;
    fm::Recti rect;
};
}