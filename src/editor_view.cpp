#include <GL/gl.h>

#include "editor_view.h"

/** @brief do_init
  *
  * @todo: document this function
  */
void EditorView::do_init()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor( 0.7, 0.7, 0.8, 1.0);
}

/** @brief do_render
  *
  * @todo: document this function
  */
void EditorView::do_render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    grid_->render();
}

/** @brief EditorView
  *
  * @todo: document this function
  */
EditorView::EditorView(Gtk::DrawingArea* widget):
OpenGLWidget(widget)
{
    GridColour c;
    c.r = c.b = c.g = 0.5f;
    grid_.reset(new OpenGLGrid(1.0f, 4, c));
}

/** @brief do_resize
  *
  * @todo: document this function
  */
void EditorView::do_resize(int width, int height)
{
    glViewport (0, 0, (GLfloat)width, (GLfloat)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float zoom = 1.0f;

    float num_tiles_across = 40.0f;

    float left = zoom * -(num_tiles_across / 2.0f);
    float right = zoom * (num_tiles_across / 2.0f);

    float ratio = float(height) / float(width);

    float num_tiles_up = num_tiles_across * ratio;

    float top = zoom * (num_tiles_up / 2.0f);
    float bottom = zoom * -(num_tiles_up / 2.0f);

    glOrtho(left, right, bottom, top, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

