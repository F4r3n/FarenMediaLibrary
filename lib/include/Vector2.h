#pragma once
#include <cmath>
#include <ostream>
namespace fm {
template <class T> class Vector2 {
public:
    Vector2() {
    }
    Vector2(T x, T y) {
        this->x = x;
        this->y = y;
    }

    Vector2 operator+=(Vector2&& b) {
        Vector2 a;
        a.x += b.x;
        a.y += b.y;
        return a;
    }

    Vector2 operator=(const Vector2& b) {
        x = b.x;
        y = b.y;
        return *this;
    }

    Vector2 operator+(const Vector2& b) {
        Vector2 v;
        v.x = x + b.x;
        v.y = y + b.y;
        return v;
    }
    void reset() {
        x = 0;
        y = 0;
    }

    T norme() {
        return sqrt(x * x + y * y);
    }
   
    friend std::ostream& operator<<(std::ostream &stream, const Vector2<T> vector) {
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