#pragma once

namespace fm
{
template <class T> class Rect
{
public:
    Rect()
    {
    }
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
    bool isValid()
    {
        if(x >= 0 && y >= 0 && w >= 0 && h >= 0)
            return true;
        return false;
    }
    
    template <typename U>
    bool contains(float x, float y) {
        if(this->x < x && x + this->w > x && this->y < y && this->y + this->h > y) return true;
        return false;
    }
    
    template <typename U> 
    bool contains(const Rect<U>& rect2)
    {
        if(x < rect2.x + rect2.w 
            && x + w > rect2.x 
            && y < rect2.y + rect2.h 
            && h + y > rect2.y) {
            return true;
        }
        return false;
    }

    T x = 0, y = 0, w = 0, h = 0;
};
typedef Rect<float> Rectf;
typedef Rect<double> Rectd;
typedef Rect<int> Recti;
}