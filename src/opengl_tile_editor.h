#ifndef OPENGL_TILE_EDITOR_H_INCLUDED
#define OPENGL_TILE_EDITOR_H_INCLUDED

#include <gtkmm.h>
#include <boost/shared_ptr.hpp>
#include "opengl_widget.h"

class Tile;
class OpenGLTileEditor;

class OpenGLTileEditorCanvas : public OpenGLWidget {
public:
    typedef boost::shared_ptr<OpenGLTileEditorCanvas> ptr;

    OpenGLTileEditorCanvas(Gtk::DrawingArea* area, OpenGLTileEditor* parent);

private:
    OpenGLTileEditor* parent_;
    GLuint tile_texture_;

    void do_render();
    void do_resize(int width, int height);
};

class OpenGLTileEditor {
public:
    typedef boost::shared_ptr<OpenGLTileEditor> ptr;

    OpenGLTileEditor(Tile* tile);
    int run();

    Tile* get_tile() { return tile_; }
private:
    Tile* tile_;

    Gtk::Dialog* dialog_;
    Gtk::DrawingArea* tile_editor_canvas_;

    OpenGLTileEditorCanvas::ptr canvas_;
};

#endif // OPENGL_TILE_EDITOR_H_INCLUDED
