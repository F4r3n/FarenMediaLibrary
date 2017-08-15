#include "gtest/gtest.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Functions.h"
#include "Core/Math/Vector2.h"
#include "Core/QuadTree.h"

using namespace fm;
using namespace math;
class QuadTreeTests : public ::testing::Test {
public:
};

TEST_F(QuadTreeTests, Insert) {
double a;
Bounds bounds(vec2(0,0),vec2(100,100));
QuadTree<double> q = QuadTree<double>(bounds);
}

