#pragma once
#include <cassert>
#include "Vector4.h"
#include "Functions.h"
namespace fm {

namespace math {
template <typename T> using col2 = vec<T, 2>;
template <typename T> struct matrix22 {
    col2<T> m[2];

    col2<T> const& operator[](int index) const;
    col2<T>& operator[](int index);
};

template <typename T> col2<T> const& matrix22<T>::operator[](int index) const {
    return m[index];
}
template <typename T> col2<T>& matrix22<T>::operator[](int index) {
    return m[index];
}
template <typename T> struct Matrix22 {
    matrix22<T> m;

    col2<T> const& operator[](int index) const
    {
        return m[index];
    }
    col2<T>& operator[](int index)
    {
        return m[index];
    }

    Matrix22(T a00,
           T a01,
           T a10,
           T a11)
    {
        m[0][0] =a00;
        m[0][1] =a01;
        m[1][0] =a10;
        m[1][1] =a11;

    }

    Matrix22(col2<T> l1, col2<T> l2);
    Matrix22(const Matrix22<T> &m)
    {
        this->m[0] = m[0];
        this->m[1] = m[1];
    }

    void identity()
    {
        m[0][0] = 1;
        m[0][1] = 0;
        m[1][0] = 0;
        m[1][1] = 1;
    }

    Matrix22()
    {
        m[0][0] = 0;
        m[0][1] = 0;
        m[1][0] = 0;
        m[1][1] = 0;
    }
};

template <typename T> Matrix22<T> operator*(const Matrix22<T>& m1, const Matrix22<T>& m2) {
    return Matrix22<T>(m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1],
                     m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1],
                     m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1],
                     m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1]
                     );
}

template <typename T> vec<T,2> operator*(const Matrix22<T>& m1, const vec<T,2>& v) {
    return vec<T,2>(m1[0][0] * v.x + m1[1][0] * v.y,
                    m1[0][1] * v.x + m1[1][1] * v.y
                     );
}
template <typename T> vec<T,2> rotate(float angle, const vec<T, 2>& v) {
    T a = cos<T>(angle);
    T b = sin<T>(angle);
     Matrix22<T> m;
     m[0][0] = m[1][1] = a;
     m[0][1] = -b;
     m[1][0] = b;
     return m*v;
}


    template <typename T> using col = vec<T, 4>;
    template <typename T> struct matrix {
        col<T> m[4];

        col<T> const& operator[](int index) const;
        col<T>& operator[](int index);
    };

    template <typename T> col<T> const& matrix<T>::operator[](int index) const {
        return m[index];
    }
    template <typename T> col<T>& matrix<T>::operator[](int index) {
        return m[index];
    }
    template <typename T> struct Matrix {
        matrix<T> m;

        col<T> const& operator[](int index) const;
        col<T>& operator[](int index);
        Matrix(T a00,
               T a01,
               T a02,
               T a03,
               T a10,
               T a11,
               T a12,
               T a13,
               T a20,
               T a21,
               T a22,
               T a23,
               T a30,
               T a31,
               T a32,
               T a33);

        Matrix(col<T> l1, col<T> l2, col<T> l3, col<T> l4);
        Matrix(const Matrix<T> &m);

        Matrix(T a)
        {
            this->m[0][0] = a;
            this->m[0][1] = a;
            this->m[0][2] = a;
            this->m[0][3] = a;

            this->m[1][0] = a;
            this->m[1][1] = a;
            this->m[1][2] = a;
            this->m[1][3] = a;

            this->m[2][0] = a;
            this->m[2][1] = a;
            this->m[2][2] = a;
            this->m[2][3] = a;

            this->m[3][0] = a;
            this->m[3][1] = a;
            this->m[3][2] = a;
            this->m[3][3] = a;
        }

        void identity();
        Matrix();
    };
    template <typename T>
    Matrix<T>::Matrix(const Matrix<T> &m) {
        this->m[0] = m[0];
        this->m[1] = m[1];
        this->m[2] = m[2];
        this->m[3] = m[3];
    }
    template <typename T> Matrix<T>::Matrix(col<T> l1, col<T> l2, col<T> l3, col<T> l4) {
        this->m[0] = l1;
        this->m[1] = l2;
        this->m[2] = l3;
        this->m[3] = l4;
    }

    template <typename T> Matrix<T> translate(const Matrix<T>& mat, const vec<T, 3>& t) {
        Matrix<T> m(mat);
        m[3] = m[0] * t[0] + m[1] * t[1] + m[2] * t[2] + m[3];
        return m;
    }

    template <typename T> Matrix<T> rotate(const Matrix<T>& m, float angle, const vec<T, 3>& v) {
        T const a = angle;
        T const c = cos<T>(a);
        T const s = sin<T>(a);

        vec<T, 3> axis(normalize(v));
        vec<T, 3> temp((T(1) - c) * axis);

        Matrix<T> Rotate;
        Rotate[0][0] = c + temp[0] * axis[0];
        Rotate[0][1] = temp[0] * axis[1] + s * axis[2];
        Rotate[0][2] = temp[0] * axis[2] - s * axis[1];

        Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
        Rotate[1][1] = c + temp[1] * axis[1];
        Rotate[1][2] = temp[1] * axis[2] + s * axis[0];

        Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
        Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
        Rotate[2][2] = c + temp[2] * axis[2];

        Matrix<T> Result;
        Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];
        Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
        Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
        Result[3] = m[3];
        return Result;
    }

    template <typename T> Matrix<T> scale(const Matrix<T>& m, const vec<T, 3>& v) {
        Matrix<T> Result;
        Result[0] = m[0] * v[0];
        Result[1] = m[1] * v[1];
        Result[2] = m[2] * v[2];
        Result[3] = m[3];
        return Result;
    }

    template<typename T>
       Matrix<T> perspective(T fovy, T aspect, T zNear, T zFar)
        {
            assert(abs(aspect - std::numeric_limits<T>::epsilon()) > static_cast<T>(0));

            T const tanHalfFovy = tan(fovy / static_cast<T>(2));

            Matrix<T> Result;
            Result[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
            Result[1][1] = static_cast<T>(1) / (tanHalfFovy);
            Result[2][2] = - (zFar + zNear) / (zFar - zNear);
            Result[2][3] = - static_cast<T>(1);
            Result[3][2] = - (static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
            return Result;
        }

    template <typename T> Matrix<T> ortho(T left, T right, T bottom, T top, T zNear, T zFar) {
        Matrix<T> Result;

        Result[0][0] = static_cast<T>(2) / (right - left);
        Result[1][1] = static_cast<T>(2) / (top - bottom);
        Result[3][0] = -(right + left) / (right - left);
        Result[3][1] = -(top + bottom) / (top - bottom);

        Result[2][2] = -static_cast<T>(2) / (zFar - zNear);
        Result[3][2] = -(zNear + zFar) / (zFar - zNear);

        return Result;
    }
    
    template <typename T> Matrix<T> ortho
	(
		T left, T right,
		T bottom, T top
	)
	{
		Matrix<T> Result;
		Result[0][0] = static_cast<T>(2) / (right - left);
		Result[1][1] = static_cast<T>(2) / (top - bottom);
		Result[2][2] = - static_cast<T>(1);
		Result[3][0] = - (right + left) / (right - left);
		Result[3][1] = - (top + bottom) / (top - bottom);
		return Result;
	}

       template<typename T>
           Matrix<T> lookAt(vec<T,3> const& eye, vec<T, 3> const& center, vec<T, 3> const& up)
           {
               vec<T, 3> const f(normalize(center - eye));
               vec<T, 3> const s(normalize(cross(f, up)));
               vec<T, 3> const u(cross(s, f));

               Matrix<T> Result(1);
               Result[0][0] = s.x;
               Result[1][0] = s.y;
               Result[2][0] = s.z;
               Result[0][1] = u.x;
               Result[1][1] = u.y;
               Result[2][1] = u.z;
               Result[0][2] =-f.x;
               Result[1][2] =-f.y;
               Result[2][2] =-f.z;
               Result[3][0] =-dot(s, eye);
               Result[3][1] =-dot(u, eye);
               Result[3][2] = dot(f, eye);

               Result[0][3] = 0;
               Result[1][3] = 0;
               Result[2][3] = 0;

               return Result;
           }


    template <typename T>
    Matrix<T>::Matrix(T a00,
                      T a01,
                      T a02,
                      T a03,
                      T a10,
                      T a11,
                      T a12,
                      T a13,
                      T a20,
                      T a21,
                      T a22,
                      T a23,
                      T a30,
                      T a31,
                      T a32,
                      T a33) {
        this->m[0][0] = a00;
        this->m[0][1] = a01;
        this->m[0][2] = a02;
        this->m[0][3] = a03;

        this->m[1][0] = a10;
        this->m[1][1] = a11;
        this->m[1][2] = a12;
        this->m[1][3] = a13;

        this->m[2][0] = a20;
        this->m[2][1] = a21;
        this->m[2][2] = a22;
        this->m[2][3] = a23;

        this->m[3][0] = a30;
        this->m[3][1] = a31;
        this->m[3][2] = a32;
        this->m[3][3] = a33;
    }

    template <typename T> Matrix<T>::Matrix() {
        identity();
    }

    template <typename T> col<T> const& Matrix<T>::operator[](int index) const {
        return m[index];
    }
    template <typename T> col<T>& Matrix<T>::operator[](int index) {
        return m[index];
    }

    template <typename T> void Matrix<T>::identity() {
        m[0][0] = 1;
        m[0][1] = 0;
        m[0][2] = 0;
        m[0][3] = 0;

        m[1][0] = 0;
        m[1][1] = 1;
        m[1][2] = 0;
        m[1][3] = 0;

        m[2][0] = 0;
        m[2][1] = 0;
        m[2][2] = 1;
        m[2][3] = 0;

        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;
    }

    template <typename T> Matrix<T> operator*(const Matrix<T>& m1, const Matrix<T>& m2) {
        return Matrix<T>(m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2] + m1[3][0] * m2[0][3],
                         m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2] + m1[3][1] * m2[0][3],
                         m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2] + m1[3][2] * m2[0][3],
                         m1[0][3] * m2[0][0] + m1[1][3] * m2[0][1] + m1[2][3] * m2[0][2] + m1[3][3] * m2[0][3],

                         m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2] + m1[3][0] * m2[1][3],
                         m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2] + m1[3][1] * m2[1][3],
                         m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2] + m1[3][2] * m2[1][3],
                         m1[0][3] * m2[1][0] + m1[1][3] * m2[1][1] + m1[2][3] * m2[1][2] + m1[3][3] * m2[1][3],

                         m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2] + m1[3][0] * m2[2][3],
                         m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2] + m1[3][1] * m2[2][3],
                         m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1] + m1[2][2] * m2[2][2] + m1[3][2] * m2[2][3],
                         m1[0][3] * m2[2][0] + m1[1][3] * m2[2][1] + m1[2][3] * m2[2][2] + m1[3][3] * m2[2][3],

                         m1[0][0] * m2[3][0] + m1[1][0] * m2[3][1] + m1[2][0] * m2[3][2] + m1[3][0] * m2[3][3],
                         m1[0][1] * m2[3][0] + m1[1][1] * m2[3][1] + m1[2][1] * m2[3][2] + m1[3][1] * m2[3][3],
                         m1[0][2] * m2[3][0] + m1[1][2] * m2[3][1] + m1[2][2] * m2[3][2] + m1[3][2] * m2[3][3],
                         m1[0][3] * m2[3][0] + m1[1][3] * m2[3][1] + m1[2][3] * m2[3][2] + m1[3][3] * m2[3][3]

                         );
    }
    template <typename T> T* value_ptr(Matrix<T>& m) {
        return &(m[0].x);
    }
    template <typename T>

    T const* value_ptr(const Matrix<T>& m) {
        return &(m[0].x);
    }
    template <typename T> std::ostream& operator<<(std::ostream& stream, const Matrix<T> m) {
        stream << m[0] << "\n" << m[1] << "\n" << m[2] << "\n" << m[3];
        return stream;
    }



    
    typedef Matrix<float> mat;
    typedef Matrix22<float> mat22;

}
}
