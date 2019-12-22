#pragma once
#if DEBUG
#include <ostream>
#endif
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
    Rect(math::vec<T, 2> position, math::vec<T, 2> scale) {
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

    inline bool contains(float x, float y) 
	{
		return ((x > this->x) && (x < (this->x + this->w))) && ((y > this->y) && (y < (this->y + this->h)));
    }

	inline bool contains(const fm::math::vec2 &inVec)
	{
		return contains(inVec.x, inVec.y);
	}

    template <typename U> bool contains(const Rect<U>& rect2) {
        if(x < rect2.x + rect2.w && rect2.x < x + w && y < rect2.y + rect2.h && rect2.y < y + h) {
            return true;
        }
        return false;
    }



    T x = 0, y = 0, w = 0, h = 0;
};
#if DEBUG
    template <typename U> std::ostream& operator<<(std::ostream& stream, const Rect<U>& rect) {
        stream << rect.x << " " << rect.y << " " << rect.w << " " << rect.h;
        return stream;
    }
#endif
typedef Rect<float> Rectf;
typedef Rect<double> Rectd;
typedef Rect<int> Recti;
}