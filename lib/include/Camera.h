#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shape.h"
#include "Tag.h"
#include "ParticleGenerator.h"
#include "Vector2.h"
namespace fm
{
struct ViewPort
{
    float x;
    float y;
    float width;
    float height;
};

class Camera
{
public:
    Camera();
    /**
     * @brief Create a camera with a width and height
     * @return 
     */
    Camera(int width, int height);
    /**
     * @brief Create a camera with a width and height. The view port takes usually the same width and height, change the zoom
     * @return 
     */
    Camera(int width, int height, const ViewPort& view, float zoom = 1);
    /**
     * @brief Current projection matrix used
     * @return projection matrix glm format 
     */
    glm::mat4 getProjection() const;
    /**
     * @brief Draw any objects which have correctly implemented a drawable object
     * @param drawable
     */
    void draw(Drawable& drawable);

/**
 * @brief Apply the current view
 */
    void view();
    ~Camera();
    void setTag(int tag);
    int tag = tg::ALL;

/** 
 * @brief Move the camera to a position
 * @param x
 * @param y
 */
    void moveTo(int x, int y);
    void moveTo(Vector2f pos);
    /**
     * @brief Rotate the camera
     * @param angle in radians
     */
    void rotate(float angle);
    /**
     * @brief After each movement applied to the camera, you need to confirm them
     */
    void apply();

private:
    int width;
    int height;

    float x = 0;
    float y = 0;
    float angle = 0;

    glm::mat4 projection;
    ViewPort viewPort;

    glm::mat4 viewMatrix;
};
}
