#pragma once
#include <ostream>
#include "Math/Vector2.h"
namespace fm {

template <class T> class Rect {
public:
    Rect() {
    }
    Rect(T x, T y, T w, T h)
        : x(x)
        , y(y)
        , w(w)
        , h(h) {
    }
    Rect(math::Vector2<T> position, math::Vector2<T> scale) {
        x = position.x;
        y = position.y;
        w = scale.x;
        h = scale.y;
    }
    ~Rect() {
    }
    bool isValid() {
        if(x >= 0 && y >= 0 && w >= 0 && h >= 0)
            return true;
        return false;
    }

    template <typename U> bool contains(float x, float y) {
        if(this->x < x && x + this->w > x && this->y < y && this->y + this->h > y)
            return true;
        return false;
    }

    template <typename U> bool contains(const Rect<U>& rect2) {
        if(x < rect2.x + rect2.w && rect2.x < x + w && y < rect2.y + rect2.h && rect2.y < y + h) {
            return true;
        }
        return false;
    }

    template <typename U> friend std::ostream& operator<<(std::ostream& stream, const Rect<U>& rect) {
        stream << rect.x << " " << rect.y << " " << rect.w << " " << rect.h;
        return stream;
    }

    T x = 0, y = 0, w = 0, h = 0;
};
typedef Rect<float> Rectf;
typedef Rect<double> Rectd;
typedef Rect<int> Recti;
}