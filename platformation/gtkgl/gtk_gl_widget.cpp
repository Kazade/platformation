#include "gtk_gl_widget.h"
#include "kazbase/logging/logging.h"

const int attributes[] = { 
    GLX_RGBA, 
    GLX_RED_SIZE, 1, 
    GLX_GREEN_SIZE, 1,
    GLX_BLUE_SIZE, 1, 
    GLX_DOUBLEBUFFER, True, 
    GLX_DEPTH_SIZE, 24, 
    None 
};

static XVisualInfo *
gdk_x11_visual_get_xvinfo (GdkScreen *screen,
                           GdkVisual *visual)
{
  Display *xdisplay;
  XVisualInfo xvinfo_template;
  XVisualInfo *xvinfo_list;
  int nitems_return;

  xdisplay = GDK_SCREEN_XDISPLAY (screen);
  xvinfo_template.visualid = XVisualIDFromVisual (GDK_VISUAL_XVISUAL (visual));
  xvinfo_template.screen = GDK_SCREEN_XNUMBER (screen);

  xvinfo_list = XGetVisualInfo (xdisplay,
                                VisualIDMask | VisualScreenMask,
                                &xvinfo_template,
                                &nitems_return);

  /* Returned XVisualInfo needs to be unique */
  g_assert (xvinfo_list != NULL && nitems_return == 1);

  return xvinfo_list;
}

GtkGLWidget::GtkGLWidget(Gtk::DrawingArea* area):
    area_(area) {
    
    area_->set_double_buffered(false);
    
    Glib::RefPtr<Gdk::Display> display = Gdk::Display::get_default();
    Glib::RefPtr<Gdk::Screen> screen = display->get_default_screen();
    Glib::RefPtr<Gdk::Visual> visual = screen->get_rgba_visual();
    Glib::RefPtr<Gdk::Colormap> colourmap = Gdk::Colormap::create(visual, false);
    
    area->set_colormap(colourmap);
    
    Display* xdisplay = GDK_DISPLAY_XDISPLAY(display->gobj());
    
    context_ = glXCreateContext (xdisplay, gdk_x11_visual_get_xvinfo(screen->gobj(), visual->gobj()), NULL, TRUE);
    
    area_->add_events(
        Gdk::EXPOSURE_MASK |
        Gdk::BUTTON_PRESS_MASK | 
        Gdk::BUTTON_RELEASE_MASK |
        Gdk::POINTER_MOTION_MASK | 
        Gdk::POINTER_MOTION_HINT_MASK |
        Gdk::KEY_PRESS_MASK | 
        Gdk::KEY_RELEASE_MASK
    );
    
    area_->signal_realize().connect(sigc::mem_fun(this, &GtkGLWidget::on_area_realize));
    //area_->signal_draw().connect(sigc::mem_fun(this, &GtkGLWidget::on_area_draw));
    area_->signal_expose_event().connect(sigc::mem_fun(this, &GtkGLWidget::on_area_expose));
    area_->signal_configure_event().connect(sigc::mem_fun(this, &GtkGLWidget::on_area_configure));
    idle_connection_ = Glib::signal_timeout().connect(sigc::mem_fun(this, &GtkGLWidget::on_area_idle), 10);
}

bool GtkGLWidget::make_current() {
    Glib::RefPtr<Gdk::Window> window = area_->get_window();
    Display* xdisplay = gdk_x11_drawable_get_xdisplay(window->gobj());
    int id = gdk_x11_drawable_get_xid(window->gobj());    
    return glXMakeCurrent(xdisplay, id, context_) == TRUE;
}

bool GtkGLWidget::on_area_idle() {
    area_->queue_draw();
    return true;
}

void GtkGLWidget::on_area_realize() {
    if(make_current()) {
        L_DEBUG("Initializing a GL widget");
        
        glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
        glEnable (GL_DEPTH_TEST);
        glDepthFunc (GL_LEQUAL);
        glEnable (GL_CULL_FACE);
        glCullFace (GL_BACK);
        glDisable (GL_DITHER);
        glShadeModel (GL_SMOOTH);
        
        do_init();
    }
}

/*
bool GtkGLWidget::on_area_draw(const ::Cairo::RefPtr< ::Cairo::Context>& cr) {
    
    
    return true;
}

*/
bool GtkGLWidget::on_area_expose(GdkEventExpose *event) {
    if(event->count > 0) return true;
    
    if(make_current()) {
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        do_render();
        
        swap_gl_buffers();
    }
    
    return true;
}

bool GtkGLWidget::on_area_configure(GdkEventConfigure* event) {
    Gtk::Allocation allocation = area_->get_allocation();
    if(make_current()) {
        glViewport (0, 0, allocation.get_width(), allocation.get_height());
        do_resize(allocation.get_width(), allocation.get_height());
    }
    return true;
}

void GtkGLWidget::swap_gl_buffers() {
    if(!make_current()) return;
    
    Glib::RefPtr<Gdk::Window> window = area_->get_window();
    Display* xdisplay = gdk_x11_drawable_get_xdisplay(window->gobj());
    int id = gdk_x11_drawable_get_xid(window->gobj());            
    glXSwapBuffers(xdisplay, id);
}
