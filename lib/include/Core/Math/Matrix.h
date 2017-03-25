#pragma once
#include "Vector4.h"
namespace fm {

namespace math {
    template <typename T> struct Matrix<T> {
        typedef vec<T, 4> col;
        col m[4];
    }
}
}