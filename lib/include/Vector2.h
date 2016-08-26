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

		Vector2 operator+=(Vector2 &&b) {
			Vector2 a;
			a.x += b.x;
			a.y += b.y;
			return a;
		}

		T x = 0, y = 0;


};
}