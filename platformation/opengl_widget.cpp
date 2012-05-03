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
void OpenGLWidget::initialize_context()
{
    assert(widget_);

    Glib::RefPtr<Gdk::GL::Config> gl_config;
    gl_config = Gdk::GL::Config::create(Gdk::GL::MODE_RGBA | Gdk::GL::MODE_ALPHA | Gdk::GL::MODE_DEPTH  | Gdk::GL::MODE_DOUBLE);

    assert(gl_config);

    //Set the widget to be gl enabled
    Gtk::GL::widget_set_gl_capability(*widget_, gl_config);
    widget_->add_events(Gdk::POINTER_MOTION_MASK | Gdk::BUTTON1_MOTION_MASK | Gdk::BUTTON3_MOTION_MASK |
                                    Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::SCROLL_MASK |
                                    Gdk::KEY_PRESS_MASK | Gdk::VISIBILITY_NOTIFY_MASK);


    widget_->signal_realize().connect(sigc::mem_fun(this, &OpenGLWidget::on_realize));
    widget_->signal_configure_event().connect(sigc::mem_fun(this, &OpenGLWidget::on_configure));
    widget_->signal_expose_event().connect(sigc::mem_fun(this, &OpenGLWidget::on_expose));
    widget_->signal_motion_notify_event().connect(sigc::mem_fun(this, &OpenGLWidget::on_motion_notify));
    widget_->signal_button_press_event().connect(sigc::mem_fun(this, &OpenGLWidget::on_button_press));
    widget_->signal_button_release_event().connect(sigc::mem_fun(this, &OpenGLWidget::on_button_release));
    widget_->signal_scroll_event().connect(sigc::mem_fun(this, &OpenGLWidget::on_scroll));

    idle_connection_ = Glib::signal_idle().connect(sigc::mem_fun(this, &OpenGLWidget::on_idle));
}

/** @brief OpenGLWidget
  *
  * @todo: document this function
  */
OpenGLWidget::OpenGLWidget(Gtk::DrawingArea* widget):
widget_(widget)
{
    initialize_context();
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
bool OpenGLWidget::on_motion_notify(GdkEventMotion* event)
{
    do_motion(event);
    return true;
}

/** @brief on_expose
  *
  * @todo: document this function
  */
bool OpenGLWidget::on_expose(GdkEventExpose *event)
{
    MakeCurrent context(this);

    if(!context.ok) {
        return true;
    }

    do_render();

    context.drawable_->swap_buffers();

    return true;
}

/** @brief on_realize
  *
  * @todo: document this function
  */
void OpenGLWidget::on_realize()
{
    MakeCurrent context(this);

    if(!context.ok) {
        return;
    }

    do_init();
}

/** @brief on_configure
  *
  * @todo: document this function
  */
bool OpenGLWidget::on_configure(GdkEventConfigure* event)
{
    MakeCurrent context(this);

    if(!context.ok) {
        return true;
    }

    do_resize(event->width, event->height);

    return true;
}

/** @brief on_idle
  *
  * @todo: document this function
  */
bool OpenGLWidget::on_idle()
{
    widget_->queue_draw();
    return true;
}

/** @brief unproject
  *
  * @todo: document this function
  */
kmVec2 OpenGLWidget::unproject(gdouble winx, gdouble winy)
{
    GLdouble winz = 0.0;

    MakeCurrent context(this);

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



