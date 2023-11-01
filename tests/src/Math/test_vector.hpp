#pragma once
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector4.h"

#include "doctest/doctest.h"

TEST_SUITE("Vector3") {
	TEST_CASE("Add") {
		fm::math::vec3 a;
		fm::math::vec3 b;

		CHECK(a.x == 0);
		CHECK(a.y == 0);
		CHECK(a.z == 0);

		CHECK_EQ(a, b);

		b.x = 1;
		b.y = 2;
		b.z = 3;

		CHECK_EQ(a + b, b);
		CHECK(b.x == 1);
		CHECK(b.y == 2);
		CHECK(b.z == 3);
	}
}

