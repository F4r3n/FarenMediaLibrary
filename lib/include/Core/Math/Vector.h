#pragma once
#include <Box2D/Common/b2Math.h>

namespace fm {
namespace math {
    template <typename T, std::size_t size> struct vec;

    template <typename T> struct vec<T, 1> {
        T x;
        operator T() {
            return x;
        }
    };

    template <typename T> struct vec<T, 2> {
        T x, y;
        vec<T, 2>(T x, T y) {
            this->x = x;
            this->y = y;
        }

        vec<T, 2>() {
        }
        // Operator +=
        vec<T, 2>& operator+=(const vec<T, 1>& b);

        template <typename P> vec<T, 2>& operator+=(const vec<P, 1>& b);

        vec<T, 2>& operator+=(const vec<T, 2>& b);

        template <typename P> vec<T, 2>& operator+=(const vec<P, 2>& b);

        // Operator =
        vec<T, 2>& operator=(const vec<T, 2>& b);

        // Operatot *=
        vec<T, 2>& operator*=(const vec<T, 2>& b);

        void reset() {
            x = 0;
            y = 0;
        }

        inline T norme() {
            return sqrt(x * x + y * y);
        }

        inline operator b2Vec2() {
            return b2Vec2(x, y);
        }

        friend std::ostream& operator<<(std::ostream& stream, const vec<T, 2> vector) {
            stream << vector.x << " " << vector.y;
            return stream;
        }
    };

    template <typename T> vec<T, 2> operator+(const vec<T, 2>& a, const vec<T, 2>& b) {
        return vec<T, 2>(a.x + b.x, a.y + b.y);
    }

    template <typename T> vec<T, 2> operator+(const vec<T, 2>& a, const vec<T, 1>& b) {
        return vec<T, 2>(a.x + b.x, a.y + b.x);
    }

    template <typename T> vec<T, 2>& vec<T, 2>::operator+=(const vec<T, 1>& b) {
        this->x += b.x;
        this->y += b.x;
        return *this;
    }

    template <typename T> template <typename P> vec<T, 2>& vec<T, 2>::operator+=(const vec<P, 1>& b) {
        this->x += static_cast<T>(b.x);
        this->y += static_cast<T>(b.x);
        return *this;
    }

    template <typename T> vec<T, 2>& vec<T, 2>::operator+=(const vec<T, 2>& b) {
        this->x += b.x;
        this->y += b.y;
        return *this;
    }

    template <typename T> template <typename P> vec<T, 2>& vec<T, 2>::operator+=(const vec<P, 2>& b) {
        this->x += static_cast<T>(b.x);
        this->y += static_cast<T>(b.y);
        return *this;
    }
    template <typename T> vec<T, 2>& vec<T, 2>::operator=(const vec<T, 2>& b) {
        this->x = b.x;
        this->y = b.y;
        return *this;
    }

    template <typename T> vec<T, 2>& vec<T, 2>::operator*=(const vec<T, 2>& b) {
        this->x *= b.x;
        this->y *= b.y;
        return *this;
    }

    template <typename T> vec<T, 2> operator*(const vec<T, 2>& a, const vec<T, 2>& b) {
        return vec<T, 2>(a.x * b.x, a.y * b.y);
    }
    template <typename T> vec<T, 2> operator*(const vec<T, 2>& a, T b) {
        return vec<T, 2>(a.x * b, a.y * b);
    }

    template <typename T> struct vec<T, 3> {
        T x, y, z;

        vec<T, 3>(T x, T y, T z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        vec<T, 3>() {
        }
        // Operator +=
        vec<T, 3>& operator+=(const vec<T, 1>& b);

        template <typename P> vec<T, 3>& operator+=(const vec<P, 1>& b);

        vec<T, 3>& operator+=(const vec<T, 3>& b);

        template <typename P> vec<T, 3>& operator+=(const vec<P, 3>& b);

        // Operator =
        vec<T, 3>& operator=(const vec<T, 3>& b);

        // Operatot *=
        vec<T, 3>& operator*=(const vec<T, 3>& b);

        void reset() {
            x = 0;
            y = 0;
            z = 0;
        }

        inline T norme() {
            return sqrt(x * x + y * y + z * z);
        }

        friend std::ostream& operator<<(std::ostream& stream, const vec<T, 3> vector) {
            stream << vector.x << " " << vector.y << " " << vector.z;
            return stream;
        }
    };

    template <typename T> vec<T, 3> operator+(const vec<T, 3>& a, const vec<T, 3>& b) {
        return vec<T, 3>(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    template <typename T> vec<T, 3> operator+(const vec<T, 3>& a, const vec<T, 1>& b) {
        return vec<T, 3>(a.x + b.x, a.y + b.x, a.z + b.x);
    }

    template <typename T> vec<T, 3>& vec<T, 3>::operator+=(const vec<T, 1>& b) {
        this->x += b.x;
        this->y += b.x;
        this->z += b.x;
        return *this;
    }

    template <typename T> template <typename P> vec<T, 3>& vec<T, 3>::operator+=(const vec<P, 1>& b) {
        this->x += static_cast<T>(b.x);
        this->y += static_cast<T>(b.x);
        this->z += static_cast<T>(b.x);
        return *this;
    }

    template <typename T> vec<T, 3>& vec<T, 3>::operator+=(const vec<T, 3>& b) {
        this->x += b.x;
        this->y += b.y;
        this->z += b.z;
        return *this;
    }

    template <typename T> template <typename P> vec<T, 3>& vec<T, 3>::operator+=(const vec<P, 3>& b) {
        this->x += static_cast<T>(b.x);
        this->y += static_cast<T>(b.y);
        this->z += static_cast<T>(b.z);
        return *this;
    }
    template <typename T> vec<T, 3>& vec<T, 3>::operator=(const vec<T, 3>& b) {
        this->x = b.x;
        this->y = b.y;
        this->z = b.z;
        return *this;
    }

    template <typename T> vec<T, 3>& vec<T, 3>::operator*=(const vec<T, 3>& b) {
        this->x *= b.x;
        this->y *= b.y;
        this->z *= b.z;
        return *this;
    }

    template <typename T> vec<T, 3> operator*(const vec<T, 3>& a, const vec<T, 3>& b) {
        return vec<T, 3>(a.x * b.x, a.y * b.y, a.z * b.z);
    }
    template <typename T> vec<T, 3> operator*(const vec<T, 3>& a, T b) {
        return vec<T, 3>(a.x * b, a.y * b, a.z * b);
    }

    template <typename T> struct vec<T, 4> {
        T x, y, z, w;
        vec<T, 4>(T x, T y, T z, T w) {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        vec<T, 4>() {
        }
        // Operator +=
        vec<T, 4>& operator+=(const vec<T, 1>& b);

        template <typename P> vec<T, 4>& operator+=(const vec<P, 1>& b);

        vec<T, 4>& operator+=(const vec<T, 4>& b);

        template <typename P> vec<T, 4>& operator+=(const vec<P, 4>& b);

        // Operator =
        vec<T, 4>& operator=(const vec<T, 4>& b);

        // Operatot *=
        vec<T, 4>& operator*=(const vec<T, 4>& b);

        void reset() {
            x = 0;
            y = 0;
            z = 0;
            w = 0;
        }

        inline T norme() {
            return sqrt(x * x + y * y + z * z + w * w);
        }

        friend std::ostream& operator<<(std::ostream& stream, const vec<T, 4> vector) {
            stream << vector.x << " " << vector.y << " " << vector.z << " " << vector.w;
            return stream;
        }

        T const& operator[](unsigned int index) const;
        T& operator[](unsigned int index);
    };

    template <typename T> vec<T, 4> operator+(const vec<T, 4>& a, const vec<T, 4>& b) {
        return vec<T, 4>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
    }

    template <typename T> vec<T, 4> operator+(const vec<T, 4>& a, const vec<T, 1>& b) {
        return vec<T, 4>(a.x + b.x, a.y + b.x, a.z + b.x, a.w + b.x);
    }

    template <typename T> vec<T, 4>& vec<T, 4>::operator+=(const vec<T, 1>& b) {
        this->x += b.x;
        this->y += b.x;
        this->z += b.x;
        this->w += b.w;
        return *this;
    }

    template <typename T> template <typename P> vec<T, 4>& vec<T, 4>::operator+=(const vec<P, 1>& b) {
        this->x += static_cast<T>(b.x);
        this->y += static_cast<T>(b.x);
        this->z += static_cast<T>(b.x);
        this->w += static_cast<T>(b.x);
        return *this;
    }

    template <typename T> vec<T, 4>& vec<T, 4>::operator+=(const vec<T, 4>& b) {
        this->x += b.x;
        this->y += b.y;
        this->z += b.z;
        this->w += b.w;
        return *this;
    }

    template <typename T> template <typename P> vec<T, 4>& vec<T, 4>::operator+=(const vec<P, 4>& b) {
        this->x += static_cast<T>(b.x);
        this->y += static_cast<T>(b.y);
        this->z += static_cast<T>(b.z);
        this->w += static_cast<T>(b.w);
        return *this;
    }
    template <typename T> vec<T, 4>& vec<T, 4>::operator=(const vec<T, 4>& b) {
        this->x = b.x;
        this->y = b.y;
        this->z = b.z;
        this->w = b.w;
        return *this;
    }

    template <typename T> vec<T, 4>& vec<T, 4>::operator*=(const vec<T, 4>& b) {
        this->x *= b.x;
        this->y *= b.y;
        this->z *= b.z;
        this->w *= b.w;
        return *this;
    }

    template <typename T> vec<T, 4> operator*(const vec<T, 4>& a, const vec<T, 4>& b) {
        return vec<T, 4>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
    }
    template <typename T> vec<T, 4> operator*(const vec<T, 4>& a, T b) {
        return vec<T, 4>(a.x * b, a.y * b, a.z * b, a.w * b);
    }

    template <typename T, typename P> vec<T, 4> operator*(const vec<T, 4>& a, P b) {
        return vec<T, 4>(
            a.x * static_cast<T>(b), a.y * static_cast<T>(b), a.z * static_cast<T>(b), a.w * static_cast<T>(b));
    }

    template <typename T> bool operator==(const vec<T, 4>& a, const vec<T, 4>& b) {
        return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
    }

    template <typename T> bool operator!=(const vec<T, 4>& a, const vec<T, 4>& b) {
        return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
    }

    template <typename T> T const& vec<T,4>::operator[](unsigned int index) const {
        return (&x)[index];
    }

    template <typename T> T& vec<T,4>::operator[](unsigned int index) {
        return (&x)[index];
    }
}
}