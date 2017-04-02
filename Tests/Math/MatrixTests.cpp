#include "gtest/gtest.h"
#include "Core/Math/Matrix.h"
#include "Core/Math/Vector3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace fm;
using namespace math;
class MatrixTest : public ::testing::Test {
public:
};

TEST_F(MatrixTest, Set) {
    mat a;
    a.identity();
    a[0][0] = 1;
    ASSERT_TRUE(a[0][0] == 1);
}
TEST_F(MatrixTest, Fetch) {
    mat a;
    a.identity();
    ASSERT_TRUE(a[0][0] == 1);
}

TEST_F(MatrixTest, Translate) {
    mat a;
    a.identity();
    a = translate(a, math::vec3(1, 1, 1));

    glm::mat4 ga;
    ga = glm::translate(ga, glm::vec3(1, 1, 1));

    ASSERT_TRUE(a[3][0] == ga[3][0] && a[3][1] == ga[3][1] && a[3][2] == ga[3][2]);
}

TEST_F(MatrixTest, Rotate) {
    mat a;
    a = rotate(a, 10.0f, math::vec3(1, 0, 0));

    glm::mat4 ga;
    ga = glm::rotate(ga, 10.0f, glm::vec3(1, 0, 0));

    ASSERT_TRUE(a[0][0] == ga[0][0] && a[0][1] == ga[0][1] && a[0][2] == ga[0][2]);
    ASSERT_TRUE(a[1][0] == ga[1][0] && a[1][1] == ga[1][1] && a[1][2] == ga[1][2]);
    ASSERT_TRUE(a[2][0] == ga[2][0] && a[2][1] == ga[2][1] && a[2][2] == ga[2][2]);
    ASSERT_TRUE(a[3][0] == ga[3][0] && a[3][1] == ga[3][1] && a[3][2] == ga[3][2]);
}

TEST_F(MatrixTest, Ortho) {
    mat a;
    a = ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.0f, 100.0f);

    glm::mat4 ga;
    ga = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.0f, 100.0f);

    ASSERT_TRUE(a[0][0] == ga[0][0] && a[0][1] == ga[0][1] && a[0][2] == ga[0][2]);
    ASSERT_TRUE(a[1][0] == ga[1][0] && a[1][1] == ga[1][1] && a[1][2] == ga[1][2]);
    ASSERT_TRUE(a[2][0] == ga[2][0] && a[2][1] == ga[2][1] && a[2][2] == ga[2][2]);
    ASSERT_TRUE(a[3][0] == ga[3][0] && a[3][1] == ga[3][1] && a[3][2] == ga[3][2]);
}

TEST_F(MatrixTest, Mult) {
    mat a;
    a = mat(0,0,1,1,
            1,1,1,1,
            1,1,1,1,
            1,1,1,1)
                ;
    mat b = mat(2,1,1,1,
            1,0,1,1,
            2,0,4,1,
            1,1,1,1);

    a = a*b;
    glm::mat4 ga;
    ga = glm::mat4(0,0,1,1,
            1,1,1,1,
            1,1,1,1,
            1,1,1,1);
    glm::mat4 gb = glm::mat4(2,1,1,1,
            1,0,1,1,
            2,0,4,1,
            1,1,1,1);
    ga = ga*gb;
   /* std::cout << a << "\n"<< std::endl;
    
    std::cout << ga[0][0] << " " << ga[0][1] << " "<< ga[0][2] << " "<< ga[0][3] << "\n"
              << ga[1][0] << " " << ga[1][1] << " "<< ga[1][2] << " "<< ga[1][3] << "\n"
              << ga[2][0] << " " << ga[2][1] << " "<< ga[2][2] << " "<< ga[2][3] << "\n"
              << ga[3][0] << " " << ga[3][1] << " "<< ga[3][2] << " "<< ga[3][3] << "\n" << std::endl;*/

    ASSERT_TRUE(a[0][0] == ga[0][0] && a[0][1] == ga[0][1] && a[0][2] == ga[0][2]);
    ASSERT_TRUE(a[1][0] == ga[1][0] && a[1][1] == ga[1][1] && a[1][2] == ga[1][2]);
    ASSERT_TRUE(a[2][0] == ga[2][0] && a[2][1] == ga[2][1] && a[2][2] == ga[2][2]);
    ASSERT_TRUE(a[3][0] == ga[3][0] && a[3][1] == ga[3][1] && a[3][2] == ga[3][2]);
}

TEST_F(MatrixTest, Scale) {
    mat a;
   glm::mat4 ga;
   a = scale(a, vec3(2,1,2));
   ga = glm::scale(ga, glm::vec3(2,1,2));

    ASSERT_TRUE(a[0][0] == ga[0][0] && a[0][1] == ga[0][1] && a[0][2] == ga[0][2]);
    ASSERT_TRUE(a[1][0] == ga[1][0] && a[1][1] == ga[1][1] && a[1][2] == ga[1][2]);
    ASSERT_TRUE(a[2][0] == ga[2][0] && a[2][1] == ga[2][1] && a[2][2] == ga[2][2]);
    ASSERT_TRUE(a[3][0] == ga[3][0] && a[3][1] == ga[3][1] && a[3][2] == ga[3][2]);
}

TEST_F(MatrixTest, Ptr) {
    mat a;
    ASSERT_TRUE(value_ptr(a) != nullptr);
}