#include <GL/gl.h>

#include "opengl_grid.h"

/** @brief set_small_step
  *
  * @todo: document this function
  */
void OpenGLGrid::set_small_step(float x)
{
    small_step_ = x;
}

/** @brief render
  *
  * @todo: make this only render the lines that are visible!
  */
void OpenGLGrid::render()
{
    float left_start = min_x_; //-(100 * small_step_);
    float right_start = max_x_; //(100 * small_step_);
    float top_start = max_y_; //(100 * small_step_);
    float bottom_start = min_y_; //-(100 * small_step_);

    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
    glDisable(GL_TEXTURE_2D);
    glColor3f(colour_.r, colour_.g, colour_.b);

    int i = 0;
    for(float x = left_start; x <= right_start; x += small_step_) {
        glLineWidth(1);
        if(i % big_step_frequency_ == 0) {
            glLineWidth(2);
        }

        glBegin(GL_LINES);
            glVertex2f(x, top_start);
            glVertex2f(x, bottom_start);
        glEnd();

        i++;
    }

    i = 0;
    for(float y = bottom_start; y <= top_start; y += small_step_) {
        glLineWidth(1);
        if(i % big_step_frequency_ == 0) {
            glLineWidth(2);
        }

        glBegin(GL_LINES);
            glVertex2f(left_start, y);
            glVertex2f(right_start, y);
        glEnd();

        i++;
    }

    glPopAttrib();
}

/** @brief OpenGLGrid
  *
  * @todo: document this function
  */
OpenGLGrid::OpenGLGrid(float small_step, int big_step_every, const GridColour& c):
small_step_(small_step),
big_step_frequency_(big_step_every),
colour_(c),
min_x_(0.0f),
max_x_(100.0f),
min_y_(-50.0f),
max_y_(50.0f)
{
    assert(small_step > 0.0f);
    assert(big_step_every > 0);
}

/** @brief set_colour
  *
  * @todo: document this function
  */
void OpenGLGrid::set_colour(const GridColour& c)
{
    colour_ = c;
}

/** @brief set_big_step_frequency
  *
  * @todo: document this function
  */
void OpenGLGrid::set_big_step_frequency(int i)
{
    big_step_frequency_ = i;
}

/** @brief snap_to
  *
  * @todo: document this function
  */
void OpenGLGrid::snap_to(double& x, double& y)
{
    float hs = small_step_ / 2.0f;
    x = int((x + hs) / small_step_) * small_step_;
    y = int((y + hs) / small_step_) * small_step_;

    if(x < min_x_) x = min_x_;
    if(x > max_x_) x = max_x_;
    if(y < min_y_) y = min_y_;
    if(y > max_y_) y = max_x_;
}

/** @brief snap_to
  *
  * @todo: document this function
  */
void OpenGLGrid::snap_to(float& x, float& y)
{
    float hs = small_step_ / 2.0f;
    x = int((x + hs) / small_step_) * small_step_;
    y = int((y + hs) / small_step_) * small_step_;

    if(x < min_x_) x = min_x_;
    if(x > max_x_) x = max_x_;
    if(y < min_y_) y = min_y_;
    if(y > max_y_) y = max_x_;
}

