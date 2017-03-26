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

TEST_F(MathTest, Multiplication) {
	vec4 a = vec4(1,0,1,0);
	vec4 b = vec4(0,1,1,1);
	vec4 c = a * b;
	EXPECT_TRUE( c == vec4(0,0,1,0));
    
    c = a * 2;
    ASSERT_TRUE( c == vec4(2,0,2,0));
}

TEST_F(MathTest, Fetch) {
    vec4 a = vec4(1,2,3,4);
    ASSERT_TRUE(a[0] == 1 && a[1] == 2 && a[2] == 3 && a[3] == 4);
    a[0]*= 2;
    ASSERT_TRUE(a.x == 2);
}



