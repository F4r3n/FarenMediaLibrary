#pragma once
#include <cmath>
#include <ostream>
#include <Box2D/Common/b2Math.h>
namespace fm {
namespace math {
    template <class T> class Vector2 {
    public:
        Vector2() {
        }
        Vector2(T x, T y) {
            this->x = x;
            this->y = y;
        }

        inline Vector2 operator+=(Vector2&& b) {
            Vector2 a;
            a.x += b.x;
            a.y += b.y;
            return a;
        }

        inline Vector2 operator=(const Vector2& b) {
            x = b.x;
            y = b.y;
            return *this;
        }

        inline Vector2 operator*(float value) {
            x *= value;
            y *= value;
            return *this;
        }

        inline Vector2 operator+(const Vector2& b) {
            Vector2 v;
            v.x = x + b.x;
            v.y = y + b.y;
            return v;
        }
        inline void reset() {
            x = 0;
            y = 0;
        }

        inline T norme() {
            return sqrt(x * x + y * y);
        }

        inline operator b2Vec2() {
            return b2Vec2(x, y);
        }

        friend std::ostream& operator<<(std::ostream& stream, const Vector2<T> vector) {
            stream << vector.x << " " << vector.y;
            return stream;
        }

        T x = 0, y = 0;
    };
    typedef Vector2<float> Vector2f;
    typedef Vector2<int> Vector2i;
    typedef Vector2<double> Vector2d;
    typedef Vector2<unsigned int> Vector2ui;
}
}