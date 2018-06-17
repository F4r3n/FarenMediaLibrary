#pragma once
#include <ostream>
#if SIMD
#include <smmintrin.h>
#endif
#include <cmath>
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
    T x = 0, y = 0;
    vec<T, 2>(T x, T y) {
        this->x = x;
        this->y = y;
    }

    vec<T, 2>() = default;

    vec<T,2>(const vec<T,2> &b) {
        x = b.x;
        y = b.y;
    }

    vec<T,2>& operator=(T* v) {
        x = v[0];
        y = v[1];
        return *this;
    }

    // Operator +=
    vec<T, 2>& operator+=(const vec<T, 1>& b);

    template <typename P> vec<T, 2>& operator+=(const vec<P, 1>& b);

    vec<T, 2>& operator+=(const vec<T, 2>& b);

    template <typename P> vec<T, 2>& operator+=(const vec<P, 2>& b);

    template <typename P> vec<T, 2>& operator*=(const P& b);

    // Operator =
    vec<T, 2>& operator=(const vec<T, 2>& b){
        this->x = b.x;
        this->y = b.y;
        return *this;
    }

    // Operator *=
    vec<T, 2>& operator*=(const vec<T, 2>& b);

    vec<T, 2>& operator*=(const T& b);


    void reset() {
        x = 0;
        y = 0;
    }

    inline T norme() {
        return sqrt(x * x + y * y);
    }

    friend std::ostream& operator<<(std::ostream& stream, const vec<T, 2> vector) {
        stream << vector.x << " " << vector.y;
        return stream;
    }

    T const& operator[](unsigned int index) const;
    T& operator[](unsigned int index);

    operator vec<T,3>() {
        return vec<T,3>(x,y,0);
    }
    //~vec<T,2>() {}
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


template <typename T> vec<T, 2>& vec<T, 2>::operator*=(const vec<T, 2>& b) {
    this->x *= b.x;
    this->y *= b.y;
    return *this;
}

template <typename T> vec<T, 2>& vec<T, 2>::operator*=(const T& b) {
    this->x *= b;
    this->y *= b;
    return *this;
}

template <typename T> template <typename P> vec<T, 2>& vec<T, 2>::operator*=(const P& b) {
    this->x *= static_cast<T>(b);
    this->y *= static_cast<T>(b);
    return *this;
}

#if SIMD
template <typename T>
#ifdef __GNUC__
struct __attribute__((aligned (16))) vec<T, 3>
#else
_MM_ALIGN16 struct vec<T, 3>
#endif
#else
template <typename T>
 struct vec<T, 3>
#endif
{
    union
       {
           struct { T x, y, z; };
           #if SIMD
           __m128 mmvalue;
#endif
       };

    vec<T, 3>(T x, T y, T z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    vec<T, 3>(T x) {
        this->x = x;
        this->y = x;
        this->z = x;
    }
#if SIMD
    inline vec<T, 3>(__m128 m) : mmvalue(m) {}
#endif
    vec<T, 3>() {
        x = 0;
        y = 0;
        z = 0;
    }
    // Operator +=
    vec<T, 3>& operator+=(const vec<T, 1>& b);

    template <typename P> vec<T, 3>& operator+=(const vec<P, 1>& b);

    vec<T, 3>& operator+=(const vec<T, 3>& b);

    template <typename P> vec<T, 3>& operator+=(const vec<P, 3>& b);

    // Operator =
    vec<T, 3>& operator=(const vec<T, 3>& b);
    vec<T, 3>& operator/=(T b);
    // Operatot *=
    vec<T, 3>& operator*=(const vec<T, 3>& b);



    void reset() {
        x = 0;
        y = 0;
        z = 0;
    }

    inline T length() {
        return sqrt(x * x + y * y + z * z);
    }

    friend std::ostream& operator<<(std::ostream& stream, const vec<T, 3> vector) {
        stream << vector.x << " " << vector.y << " " << vector.z;
        return stream;
    }

    T const& operator[](unsigned int index) const;
    T& operator[](unsigned int index);
};

template <typename T> vec<T, 3> operator-(const vec<T, 3>& a, const vec<T, 3>& b) {
    return vec<T, 3>(a.x - b.x, a.y - b.y, a.z - b.z);
}

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

template <typename T> vec<T, 3>& vec<T, 3>::operator+=(const vec<T, 3>& b)
 {
#if SIMD
    mmvalue = _mm_add_ps(mmvalue, b.mmvalue);
#else
     this->x += b.x;
     this->y += b.y;
     this->z += b.z;
#endif
    return *this;
}

template <typename T> vec<T, 3>& vec<T, 3>::operator/=(T b) {
    this->x /= b;
    this->y /= b;
    this->z /= b;
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

#if SIMD
    mmvalue = _mm_mul_ps(mmvalue, b.mmvalue);
#else
     this->x *= b.x;
     this->y *= b.y;
     this->z *= b.z;
#endif
    return *this;
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
        x = 0;
        y = 0;
        z = 0;
        w = 0;
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
    vec<T, 4>(const vec<T, 4>& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        w = v.w;
    }

    explicit vec<T, 4>(const vec<T, 3>& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        w = 0;
    }
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

/**************OPERATOR [] **********************/
template <typename T> T const& vec<T, 4>::operator[](unsigned int index) const {
    return (&x)[index];
}

template <typename T> T& vec<T, 4>::operator[](unsigned int index) {
    return (&x)[index];
}

template <typename T> T const& vec<T, 3>::operator[](unsigned int index) const {
    return (&x)[index];
}

template <typename T> T& vec<T, 3>::operator[](unsigned int index) {
    return (&x)[index];
}

template <typename T> T const& vec<T, 2>::operator[](unsigned int index) const {
    return (&x)[index];
}

template <typename T> T& vec<T, 2>::operator[](unsigned int index) {
    return (&x)[index];
}
/***********CAST***************************/


/**************OPERATOR * ************************/

template <typename T> vec<T, 4> operator*(const vec<T, 4>& a, const vec<T, 4>& b) {
    return vec<T, 4>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}
template <typename T> vec<T, 4> operator*(const vec<T, 4>& a, T b) {
    return vec<T, 4>(a.x * b, a.y * b, a.z * b, a.w * b);
}

template <typename T> vec<T, 2> operator*(const vec<T, 2>& a, const vec<T, 2>& b) {
    return vec<T, 2>(a.x * b.x, a.y * b.y);
}
template <typename T> vec<T, 2> operator*(const vec<T, 2>& a, T b) {
    return vec<T, 2>(a.x * b, a.y * b);
}

template <typename T>
 vec<T, 3> operator*(const vec<T, 3>& a, const vec<T, 3>& b)
 {
#if !SIMD
    return vec<T, 3>(a.x * b.x, a.y * b.y, a.z * b.z);
#else
     return _mm_mul_ps(a.mmvalue, b.mmvalue);
#endif
}
template <typename T> vec<T, 3> operator*(const vec<T, 3>& a, T b) {
    return vec<T, 3>(a.x * b, a.y * b, a.z * b);
}

template <typename T> vec<T, 3> operator*(T b, const vec<T, 3>& a) {
    return vec<T, 3>(a.x * b, a.y * b, a.z * b);
}
/****************SUM*******************************/
template <typename T> T sum(const vec<T, 4>& vector) {
    return vector.x + vector.y + vector.z + vector.w;
}

template <typename T> T sum(const vec<T, 3>& vector) {
    return vector.x + vector.y + vector.z;
}

template <typename T> T sum(const vec<T, 2>& vector) {
    return vector.x + vector.y;
}

/***********DIVIDE**************************************/

template <typename T> vec<T, 2> operator/(const vec<T, 2>& a, T b) {
    return vec<T, 2>(a.x / b, a.y / b);
}
template <typename T> vec<T, 3> operator/(const vec<T, 3>& a, T b) {
    return vec<T, 3>(a.x / b, a.y / b, a.z / b);
}
 template <typename T> vec<T, 3> operator/(const vec<T, 3>& a, const vec<T, 3>& b) {
     return vec<T, 3>(a.x / b.x, a.y / b.y, a.z / b.z);
 }
template <typename T> vec<T, 4> operator/(const vec<T, 4>& a, T b) {
    return vec<T, 4>(a.x / b, a.y / b, a.z / b, a.w / b);
}

template <typename T> vec<T, 3> operator-(const vec<T, 3>& a) {
    return vec<T, 3>(-a.x, -a.y, -a.z);
}

/*****************NORMALIZE*************************/
template <typename T> vec<T, 4> normalize(const vec<T, 4>& vector) {
    T s = 1.0/sqrt(vector.x*vector.x + vector.y*vector.y + vector.z*vector.z + vector.w*vector.w);
    return vec<T, 4>(vector.x * s, vector.y * s, vector.z * s, vector.w* s);
}

template <typename T> vec<T, 3> normalize(const vec<T, 3>& vector) {
#if !SIMD
    T s = 1.0/std::sqrt(vector.x*vector.x + vector.y*vector.y + vector.z*vector.z);
    return vec<T, 3>(vector.x * s, vector.y * s, vector.z * s);
#else

     T s = 1.0/dot(vector);
     return vec<T, 3>(vector.x * s, vector.y * s, vector.z * s);
#endif
}

template <typename T> vec<T, 2> normalize(const vec<T, 2>& vector) {
    T s = 1.0/sqrt(vector.x*vector.x + vector.y*vector.y);
    return vec<T, 2>(vector.x * s, vector.y * s);
}
/*****DOT****/
template <typename T> T dot(const vec<T, 3>& a, const vec<T, 3>& b) {
#if !SIMD
    return a.x*b.x + a.y*b.y + a.z*b.z;
#else
     __m128 mulRes, shufReg, sumsReg;
         mulRes = _mm_mul_ps(a.mmvalue, b.mmvalue);

         // Calculates the sum of SSE Register - https://stackoverflow.com/a/35270026/195787
         shufReg = _mm_movehdup_ps(mulRes);        // Broadcast elements 3,1 to 2,0
         sumsReg = _mm_add_ps(mulRes, shufReg);
         shufReg = _mm_movehl_ps(shufReg, sumsReg); // High Half -> Low Half
         sumsReg = _mm_add_ss(sumsReg, shufReg);
         return  _mm_cvtss_f32(sumsReg); // Result in the lower part of the SSE Register
#endif
}
template <typename T> T dot(const vec<T, 3>& a) {
    return a.x*a.x + a.y*a.y + a.z*a.z;
}
/*****CROSS****/
template <typename T> vec<T,3> cross(const vec<T, 3>& a, const vec<T, 3>& b) {
    return vec<T,3>(a.y*b.z - b.y*a.z, a.z*b.x - b.z*a.x, a.x*b.y - a.y*b.x);
}
template <typename T>
T length(const vec<T, 3>& a) {
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}
}


}

