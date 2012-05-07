/***********************************************************************************
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published
*  by the Free Software Foundation; either version 3 of the License, or (at
*  your option) any later version.
*
*  This program is distributed in the hope that it will be useful, but
*  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
*  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
*  License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License
*  along with this program; if not, see <http://www.gnu.org/copyleft/lesser.html>.
*
**********************************************************************************/


#include <cassert>
#include <GL/glu.h>

#include "opengl_widget.h"


/** @brief initialize_context
  *
  * @todo: document this function
  */
void OpenGLWidget::initialize() {
    area()->signal_motion_notify_event().connect(sigc::mem_fun(this, &OpenGLWidget::on_motion_notify));
    area()->signal_button_press_event().connect(sigc::mem_fun(this, &OpenGLWidget::on_button_press));
    area()->signal_button_release_event().connect(sigc::mem_fun(this, &OpenGLWidget::on_button_release));
    area()->signal_scroll_event().connect(sigc::mem_fun(this, &OpenGLWidget::on_scroll));
}

/** @brief OpenGLWidget
  *
  * @todo: document this function
  */
OpenGLWidget::OpenGLWidget(Gtk::DrawingArea* widget):
GtkGLWidget(widget) {
    initialize();
}

/** @brief on_scroll
  *
  * @todo: document this function
  */
bool OpenGLWidget::on_scroll(GdkEventScroll* event)
{
    do_scroll(event);
    return true;
}

/** @brief on_button_release
  *
  * @todo: document this function
  */
bool OpenGLWidget::on_button_release(GdkEventButton* event)
{
    do_button_release(event);
    return true;
}

/** @brief on_button_press
  *
  * @todo: document this function
  */
bool OpenGLWidget::on_button_press(GdkEventButton* event)
{
    do_button_press(event);
    return true;
}

/** @brief on_motion_notify
  *
  * @todo: document this function
  */
bool OpenGLWidget::on_motion_notify(GdkEventMotion* event) {
    do_motion(event);
    return true;
}

/** @brief unproject
  *
  * @todo: document this function
  */
kmVec2 OpenGLWidget::unproject(gdouble winx, gdouble winy) {
    GLdouble winz = 0.0;

    make_current();

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    GLdouble x, y, z;

    winy = double(viewport[3]) - winy;

    glReadPixels(winx, winy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winz);
    gluUnProject(winx, winy, winz, modelview, projection, viewport, &x, &y, &z);

    kmVec2 result;
    result.x = x;
    result.y = y;

    return result;
}



