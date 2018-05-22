#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <cmath>

namespace fm {
namespace math {
    constexpr double pi() {
        return M_PI;
    }

    constexpr double halfPi() {
        return M_PI / 2.0f;
    }

    constexpr double twoPi() {
        return 2.0f * M_PI;
    }

    template <typename T> inline T mod(T a, T b) {
        return a - b * std::floor(a / b);
    }

    template <typename T> inline T wrapAngle(T angle) {
        return std::abs(mod<T>(angle, twoPi()));
    }

    // http: // www.ganssle.com/approx/approx.pdf
    template <typename T> inline T cos_52s(T x) {
        T x2 = x * x;
        return (T(0.9999932946) + x2 * (T(-0.4999124376) + x2 * (T(0.0414877472) + T(-0.0012712095) * x2)));
    }
    template <typename T> inline T fastCos(T x) {
        T const angle = wrapAngle(x);

        if(angle < 3 * halfPi())
            return -cos_52s<T>(angle - pi());
        if(angle < pi())
            return -cos_52s<T>(pi() - angle);
        if(angle < halfPi())
            return cos_52s<T>(angle);

        return cos_52s<T>(twoPi() - angle);
    }
    template <typename T> inline T fastSin(T x) {
        return fastCos<T>(halfPi() - x);
    }
    template <typename T> inline T cos(T x) {
        return std::cos(x);
    }
    template <typename T> inline T sin(T x) {
        return std::sin(x);
    }

    template <typename T> inline T tan(T x) {
        return std::tan(x);
    }
}

// TODO fast cos and sin
}
#endif
