#ifndef EDITOR_VIEW_H_INCLUDED
#define EDITOR_VIEW_H_INCLUDED

#include <boost/shared_ptr.hpp>
#include "opengl_widget.h"
#include "opengl_grid.h"

class EditorView : public OpenGLWidget {
public:
    typedef boost::shared_ptr<EditorView> ptr;

    EditorView(Gtk::DrawingArea* widget);

private:
    void do_render();
    void do_init();
    void do_resize(int width, int height);

    OpenGLGrid::ptr grid_;
};


#endif // EDITOR_VIEW_H_INCLUDED
