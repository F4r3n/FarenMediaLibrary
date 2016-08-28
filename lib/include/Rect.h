#pragma once

namespace fm {
template <class T>
	class Rect {
	public:
			Rect(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) {}
			~Rect() {}
			T x,y,w,h;
	};
typedef Rect<float> Rectf;
typedef Rect<int> Recti;
}