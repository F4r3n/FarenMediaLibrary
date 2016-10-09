#pragma once
#include "Component.h"
#include "Rect.h"
namespace fmc {
class CCollider : public Component<CCollider> {
public:
    CCollider(fm::Recti rect) {
        this->rect = rect;
    }
    ~CCollider() {
    }
    fm::Recti rect;
};
}