#pragma once

namespace fm
{
template <class T> class Rect
{
    public:
    Rect() {}
    Rect(T x, T y, T w, T h)
        : x(x)
        , y(y)
        , w(w)
        , h(h)
    {
    }
    ~Rect()
    {
    }
    T x = 0, y = 0, w = 0, h = 0;
};
typedef Rect<float> Rectf;
typedef Rect<double> Rectd;
typedef Rect<int> Recti;
}