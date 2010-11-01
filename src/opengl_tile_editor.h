#ifndef OPENGL_TILE_EDITOR_H_INCLUDED
#define OPENGL_TILE_EDITOR_H_INCLUDED

#include <gtkmm.h>
#include <boost/shared_ptr.hpp>
#include "opengl_widget.h"
#include "geometry_element.h"
#include "opengl_picker.h"

enum GeomDrawMode {
    GDM_BOX,
    GDM_TRIANGLE,
    GDM_CURVE
};

class Tile;
class OpenGLTileEditor;

class OpenGLTileEditorCanvas : public OpenGLWidget {
public:
    typedef boost::shared_ptr<OpenGLTileEditorCanvas> ptr;

    OpenGLTileEditorCanvas(Gtk::DrawingArea* area, OpenGLTileEditor* parent);

private:
    OpenGLPicker<GeometryElement::ptr>::ptr picker_;

    OpenGLTileEditor* parent_;
    GLuint tile_texture_;

    void do_render();
    void do_resize(int width, int height);
    void do_button_press(GdkEventButton* event);
    void do_motion(GdkEventMotion* event);
    void do_button_release(GdkEventButton* event);

    GeometryElement get_element_from_draw_mode(const kmVec2& v1, const kmVec2& v2);

    GeometryElement current_element_;
    kmVec2 v1, v2;
    bool drawing_;
};

class OpenGLTileEditor {
public:
    typedef boost::shared_ptr<OpenGLTileEditor> ptr;

    OpenGLTileEditor(Tile* tile);
    int run(Gtk::Window* parent=NULL);

    Tile* get_tile() { return tile_; }

    GeomDrawMode get_geom_draw_mode() const;
    GeometryLayer get_geom_layer_mode() const;

private:
    void create_dialog();

    Tile* tile_;

    Gtk::Dialog* dialog_;
    Gtk::DrawingArea* tile_editor_canvas_;

    Gtk::RadioButton* gtk_radio_triangle_;
    Gtk::RadioButton* gtk_radio_box_;
    Gtk::RadioButton* gtk_radio_curve_;
    Gtk::RadioButton* gtk_radio_back_;
    Gtk::RadioButton* gtk_radio_normal_;
    Gtk::RadioButton* gtk_radio_front_;

    OpenGLTileEditorCanvas::ptr canvas_;
};

#endif // OPENGL_TILE_EDITOR_H_INCLUDED
