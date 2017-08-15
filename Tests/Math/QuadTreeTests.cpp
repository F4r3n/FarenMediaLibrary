#include "gtest/gtest.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Functions.h"
#include "Core/Math/Vector2.h"
#include "Core/QuadTree.h"
#include <iostream>
using namespace fm;
using namespace math;
class QuadTreeTests : public ::testing::Test {
public:
};

TEST_F(QuadTreeTests, Insert) {
double a;
Bounds bounds(vec2(500,500),vec2(1000,1000));
QuadTree<double> q = QuadTree<double>(bounds);
Data<double> b;
b.load = &a;
b.pos = vec2(10,10);
//std::cout << "Is inside " << q.insert(b) << " " << bounds.isInside(vec2(10,10)) << std::endl;
Data<double> c;
c.load = &a;
c.pos = vec2(500,500);
std::cout << q.insert(c) << std::endl;

std::vector<Data<double>> v = q.queryRange(fm::Bounds(vec2(500,500), vec2(1000,1000)));
std::cout << v.size() << std::endl;
}

