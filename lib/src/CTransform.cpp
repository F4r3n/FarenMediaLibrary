#include "CTransform.h"

using namespace fmc;

CTransform::CTransform() {
}

CTransform::CTransform(const fm::Vector2f& position, 
                        const fm::Vector2f& scale, const float& rotation, const int &layer)
    : position(position)
    , scale(scale)
    , rotation(rotation), layer(layer) {
}