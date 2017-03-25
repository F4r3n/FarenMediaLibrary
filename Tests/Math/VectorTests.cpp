#include "gtest/gtest.h"
#include "Core/Math/Vector4.h"

using namespace fm;
using namespace math;
class MathTest : public ::testing::Test {
public:
    
   
};

TEST_F(MathTest, Addition) {
	vec4 a = vec4(1,0,1,0);
	vec4 b = vec4(0,1,1,1);
	vec4 c = a + b;
	ASSERT_TRUE( c == vec4(1,1,2,1));

}


