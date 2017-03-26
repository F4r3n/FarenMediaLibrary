#include "gtest/gtest.h"
#include "Core/Math/Matrix.h"

using namespace fm;
using namespace math;
class MatrixTest : public ::testing::Test {
public:
    
   
};


TEST_F(MatrixTest, Fetch) {
    mat a;
    a.identity();
    ASSERT_TRUE(a[0][0] == 1);
    //a[0];
}