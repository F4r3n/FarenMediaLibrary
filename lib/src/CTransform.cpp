#include "CTransform.h"

using namespace fmc;

CTransform::CTransform() {
}

CTransform::CTransform(const fm::Vector2f& position, const fm::Vector2f& scale, const float& rotation)
    : position(position)
    , scale(scale)
    , rotation(rotation) {
}