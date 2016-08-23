#pragma once

namespace fm {
template <class T>
class Vector2 {
	public:
		Vector2() {}
		Vector2(T x, T y) {
			this->x = x;
			this->y = y;
		}

		T x, y;


};
}