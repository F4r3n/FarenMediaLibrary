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

    for(float i = 0; i < 2*fm::math::pi(); i += 2*fm::math::pi() / 100) {
        mat a;
        a = rotate(a, i, math::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4 ga;
        ga = glm::rotate(ga, i, glm::vec3(0.0f, 0.0f, 1.0f));

        ASSERT_TRUE(a[0][0] == ga[0][0] && a[0][1] == ga[0][1] && a[0][2] == ga[0][2]);
        ASSERT_TRUE(a[1][0] == ga[1][0] && a[1][1] == ga[1][1] && a[1][2] == ga[1][2]);
        ASSERT_TRUE(a[2][0] == ga[2][0] && a[2][1] == ga[2][1] && a[2][2] == ga[2][2]);
        ASSERT_TRUE(a[3][0] == ga[3][0] && a[3][1] == ga[3][1] && a[3][2] == ga[3][2]);
    }
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
    a = mat(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    mat b = mat(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);

    a = a * b;
    glm::mat4 ga;
    ga = glm::mat4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
    glm::mat4 gb = glm::mat4(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
    ga = ga * gb;
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
    a = scale(a, vec3(2, 1, 2));
    ga = glm::scale(ga, glm::vec3(2, 1, 2));

    ASSERT_TRUE(a[0][0] == ga[0][0] && a[0][1] == ga[0][1] && a[0][2] == ga[0][2]);
    ASSERT_TRUE(a[1][0] == ga[1][0] && a[1][1] == ga[1][1] && a[1][2] == ga[1][2]);
    ASSERT_TRUE(a[2][0] == ga[2][0] && a[2][1] == ga[2][1] && a[2][2] == ga[2][2]);
    ASSERT_TRUE(a[3][0] == ga[3][0] && a[3][1] == ga[3][1] && a[3][2] == ga[3][2]);
}

TEST_F(MatrixTest, Ptr) {
    mat a;
    ASSERT_TRUE(value_ptr(a) != nullptr);
}

void display(glm::mat4 model2) {
    std::cout << model2[0][0] << " " << model2[0][1] << " "<< model2[0][2] << " "<< model2[0][3] << "\n"
               << model2[1][0] << " " << model2[1][1] << " "<< model2[1][2] << " "<< model2[1][3] << "\n"
               << model2[2][0] << " " << model2[2][1] << " "<< model2[2][2] << " "<< model2[2][3] << "\n"
               << model2[3][0] << " " << model2[3][1] << " "<< model2[3][2] << " "<< model2[3][3] << "\n" << std::endl;
}

TEST_F(MatrixTest, Transform) {
    mat model;
    vec2 worldPos = vec2(10,10);
    vec2 scale = vec2(100,100);
    float rotation = 1;
    model = fm::math::translate(model, fm::math::vec3(worldPos.x, worldPos.y, 3));
    model = fm::math::translate(model, fm::math::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f));
    model = fm::math::rotate(model, rotation, fm::math::vec3(0.0f, 0.0f, 1.0f));
    model = fm::math::translate(model, fm::math::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f));
    model = fm::math::scale(model, fm::math::vec3(scale.x, scale.y, 1.0f));
    
    glm::mat4 model2;
    glm::vec2 worldPos2 = glm::vec2(10,10);
    glm::vec2 scale2 = glm::vec2(100,100);
    model2 = glm::translate(model2, glm::vec3(worldPos2.x, worldPos2.y, 3));
    model2 = glm::translate(model2, glm::vec3(0.5f * scale2.x, 0.5f * scale2.y, 0.0f));
    model2 = glm::rotate(model2, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    model2 = glm::translate(model2, glm::vec3(-0.5f * scale2.x, -0.5f * scale2.y, 0.0f));
    model2 = glm::scale(model2, glm::vec3(scale2.x, scale2.y, 1.0f));
    

    
    ASSERT_TRUE(model[0][0] == model2[0][0] && model[0][1] == model2[0][1] && model[0][2] == model2[0][2]);
    ASSERT_TRUE(model[1][0] == model2[1][0] && model[1][1] == model2[1][1] && model[1][2] == model2[1][2]);
    ASSERT_TRUE(model[2][0] == model2[2][0] && model[2][1] == model2[2][1] && model[2][2] == model2[2][2]);
    ASSERT_TRUE(model[3][0] == model2[3][0] && model[3][1] == model2[3][1] && model[3][2] == model2[3][2]);
}
