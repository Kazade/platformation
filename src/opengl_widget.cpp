#include <cassert>

#include <gdkmm.h>
#include <gtkglmm.h>

#include "opengl_widget.h"


/** @brief initialize_context
  *
  * @todo: document this function
  */
void OpenGLWidget::initialize_context()
{
    assert(widget_);

    Glib::RefPtr<Gdk::GL::Config> gl_config;
    gl_config = Gdk::GL::Config::create(Gdk::GL::MODE_RGB | Gdk::GL::MODE_DEPTH  | Gdk::GL::MODE_DOUBLE);

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
    return true;
}

/** @brief on_button_release
  *
  * @todo: document this function
  */
bool OpenGLWidget::on_button_release(GdkEventButton* event)
{
    return true;
}

/** @brief on_button_press
  *
  * @todo: document this function
  */
bool OpenGLWidget::on_button_press(GdkEventButton* event)
{
    return true;
}

/** @brief on_motion_notify
  *
  * @todo: document this function
  */
bool OpenGLWidget::on_motion_notify(GdkEventMotion* event)
{
    return true;
}

/** @brief on_expose
  *
  * @todo: document this function
  */
bool OpenGLWidget::on_expose(GdkEventExpose *event)
{
    Glib::RefPtr<Gdk::GL::Context> context = Gtk::GL::widget_get_gl_context(*widget_);
    Glib::RefPtr<Gdk::GL::Drawable> drawable = Gtk::GL::widget_get_gl_drawable(*widget_);

    if(!drawable->gl_begin(context)) {
        return true;
    }

    do_render();

    drawable->swap_buffers();
    drawable->gl_end();

    return true;
}

/** @brief on_realize
  *
  * @todo: document this function
  */
void OpenGLWidget::on_realize()
{
    Glib::RefPtr<Gdk::GL::Context> context = Gtk::GL::widget_get_gl_context(*widget_);
    Glib::RefPtr<Gdk::GL::Drawable> drawable = Gtk::GL::widget_get_gl_drawable(*widget_);

    if(!drawable->gl_begin(context)) {
        return;
    }

    do_init();

    drawable->gl_end();
}

/** @brief on_configure
  *
  * @todo: document this function
  */
bool OpenGLWidget::on_configure(GdkEventConfigure* event)
{
    Gtk::Allocation allocation = widget_->get_allocation();

    Glib::RefPtr<Gdk::GL::Context> context = Gtk::GL::widget_get_gl_context(*widget_);
    Glib::RefPtr<Gdk::GL::Drawable> drawable = Gtk::GL::widget_get_gl_drawable(*widget_);

    if(!drawable->gl_begin(context)) {
        return true;
    }

    do_resize(allocation.get_width(), allocation.get_height());

    drawable->gl_end();

    return true;
}



