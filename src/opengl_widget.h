#ifndef OPENGL_WIDGET_H_INCLUDED
#define OPENGL_WIDGET_H_INCLUDED

#include <gtkmm.h>
#include <gdkmm.h>
#include <gtkglmm.h>
#include <GL/gl.h>

class OpenGLWidget {
public:
    OpenGLWidget(Gtk::DrawingArea* widget);
    virtual ~OpenGLWidget() {}

    void initialize_context();

    bool on_configure(GdkEventConfigure* event);
    void on_realize();
    bool on_expose(GdkEventExpose *event);
    bool on_motion_notify(GdkEventMotion* event);
    bool on_button_press(GdkEventButton* event);
    bool on_button_release(GdkEventButton* event);
    bool on_scroll(GdkEventScroll* event);
    bool on_idle();

    Gtk::DrawingArea* get_widget() { return widget_; }

private:
    Gtk::DrawingArea* widget_;

    virtual void do_render() {}
    virtual void do_init() {}
    virtual void do_resize(int width, int height) {}
    virtual void do_button_press(GdkEventButton* event) {}
    virtual void do_motion(GdkEventMotion* event) {}
protected:
    struct MakeCurrent {
        Glib::RefPtr<Gdk::GL::Context> context_;
        Glib::RefPtr<Gdk::GL::Drawable> drawable_;
        bool ok;

        MakeCurrent(OpenGLWidget* canvas):
        ok(true) {
            context_ = Gtk::GL::widget_get_gl_context(*canvas->get_widget());
            drawable_ = Gtk::GL::widget_get_gl_drawable(*canvas->get_widget());
            if(!drawable_->gl_begin(context_)) {
                ok = false;
            }
        }

        ~MakeCurrent() {
            drawable_->gl_end();
        }
    };


};

#endif // OPENGL_WIDGET_H_INCLUDED
