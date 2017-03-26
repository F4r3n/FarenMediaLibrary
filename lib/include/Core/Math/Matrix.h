#pragma once
#include "Vector4.h"
namespace fm {

namespace math {
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
        matrix<T> m_invert;
        matrix<T> m_transpose;

        col<T> const& operator[](int index) const;
        col<T>& operator[](int index);
        Matrix<T> identity();
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
        Matrix();
    };
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
    
    template <typename T>
    Matrix<T>::Matrix() {
        this->m[0][0] = 0;
        this->m[0][1] = 0;
        this->m[0][2] = 0;
        this->m[0][3] = 0;

        this->m[1][0] = 0;
        this->m[1][1] = 0;
        this->m[1][2] = 0;
        this->m[1][3] = 0;

        this->m[2][0] = 0;
        this->m[2][1] = 0;
        this->m[2][2] = 0;
        this->m[2][3] = 0;

        this->m[3][0] = 0;
        this->m[3][1] = 0;
        this->m[3][2] = 0;
        this->m[3][3] = 0;
    }

    template <typename T> col<T> const& Matrix<T>::operator[](int index) const {
        return m[index];
    }
    template <typename T> col<T>& Matrix<T>::operator[](int index) {
        return m[index];
    }

    template <typename T> Matrix<T> Matrix<T>::identity() {
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
        return *this;
    }
    typedef Matrix<float> mat;
}
}