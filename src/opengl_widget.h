#ifndef OPENGL_WIDGET_H_INCLUDED
#define OPENGL_WIDGET_H_INCLUDED

#include <gtkmm.h>
#include <GL/gl.h>

class OpenGLWidget {
public:
    OpenGLWidget(Gtk::DrawingArea* widget);

    void initialize_context();

    bool on_configure(GdkEventConfigure* event);
    void on_realize();
    bool on_expose(GdkEventExpose *event);
    bool on_motion_notify(GdkEventMotion* event);
    bool on_button_press(GdkEventButton* event);
    bool on_button_release(GdkEventButton* event);
    bool on_scroll(GdkEventScroll* event);

private:
    Gtk::DrawingArea* widget_;

    virtual void do_render() {}
    virtual void do_init() {}
    virtual void do_resize(int width, int height) {}
};

#endif // OPENGL_WIDGET_H_INCLUDED
