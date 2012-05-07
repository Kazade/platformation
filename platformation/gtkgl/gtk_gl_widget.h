#ifndef GTK_GL_WIDGET_H
#define GTK_GL_WIDGET_H

#include <gtkmm.h>
#include <tr1/memory>

#include <GL/glx.h>
#include <gdk/gdkx.h>

class GtkGLWidget {
public:
    typedef std::tr1::shared_ptr<GtkGLWidget> ptr;
    
    GtkGLWidget(Gtk::DrawingArea* area);
    virtual ~GtkGLWidget() {
        if(idle_connection_.connected()) {
            idle_connection_.disconnect();
        }
    }
    
    Gtk::DrawingArea* area() { return area_; }
    Gtk::DrawingArea* get_widget() { return area(); }
    
    void on_area_realize();
    //bool on_area_draw(const ::Cairo::RefPtr< ::Cairo::Context>& cr);
    
    bool on_area_expose(GdkEventExpose *event);
    bool on_area_configure(GdkEventConfigure* event);
    bool on_area_idle();
    
protected:
    bool make_current();
    
    virtual void do_render() {}
    virtual void do_init() {}
    virtual void do_resize(int width, int height) {}    
private:
    Gtk::DrawingArea* area_;
    GLXContext context_;
    sigc::connection idle_connection_;
};

#endif
