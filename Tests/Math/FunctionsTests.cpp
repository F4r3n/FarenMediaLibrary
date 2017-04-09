#include "gtest/gtest.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Functions.h"
#include "Core/Math/Vector2.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace fm;
using namespace math;
class FunctionsTests : public ::testing::Test {
public:
};

TEST_F(FunctionsTests, Cos) {
    std::cout << fm::math::pi() << " " << glm::pi<float>() << std::endl;
    std::cout << fm::math::cos<double>(2) << " " << glm::cos(2) << std::endl;
    for(float i = 0; i < 2*fm::math::pi(); i += 2*fm::math::pi() / 360) {
    std::cout <<i*180/pi()<< " " << fm::math::cos<double>(i) << " " << glm::cos(i) << std::endl;
	ASSERT_TRUE(fm::math::cos<double>(i)== glm::cos(i));
}
}
