#include <iostream>
#include "opengl_tile_editor.h"

#include "tile.h"

#define UI_FILE "ui/tile_editor.glade"

/** @brief OpenGLTileEditor
  *
  * @todo: document this function
  */
OpenGLTileEditor::OpenGLTileEditor(Tile* tile):
tile_(tile),
dialog_(NULL)
{
    create_dialog();
}

/** @brief get_geom_draw_mode
  *
  * @todo: document this function
  */
GeomDrawMode OpenGLTileEditor::get_geom_draw_mode() const
{
    if(gtk_radio_triangle_->get_active()) {
        return GDM_TRIANGLE;
    } else if (gtk_radio_curve_->get_active()) {
        return GDM_CURVE;
    }

    return GDM_BOX;
}
/** @brief get_geom_layer_mode
  *
  * @todo: document this function
  */
GeometryLayer OpenGLTileEditor::get_geom_layer_mode() const
{
    if(gtk_radio_back_->get_active()) {
        return GEO_BACK;
    } else if (gtk_radio_front_->get_active()) {
        return GEO_FRONT;
    }

    return GEO_NORMAL;
}


void OpenGLTileEditor::create_dialog() {
    if(dialog_) {
        return;
    }

    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
    builder->add_from_file(UI_FILE);

    builder->get_widget("tile_editor_dialog", dialog_);
    assert(dialog_);

    builder->get_widget("tile_editor_canvas", tile_editor_canvas_);
    assert(tile_editor_canvas_);

    canvas_.reset(new OpenGLTileEditorCanvas(tile_editor_canvas_, this));
    dialog_->show_all();

    builder->get_widget("radio_triangle", gtk_radio_triangle_);
    builder->get_widget("radio_box", gtk_radio_box_);
    builder->get_widget("radio_curve", gtk_radio_curve_);
    builder->get_widget("radio_front", gtk_radio_front_);
    builder->get_widget("radio_normal", gtk_radio_normal_);
    builder->get_widget("radio_back", gtk_radio_back_);

    assert(gtk_radio_triangle_);
    assert(gtk_radio_box_);
    assert(gtk_radio_curve_);
    assert(gtk_radio_back_);
    assert(gtk_radio_normal_);
    assert(gtk_radio_front_);
}

/** @brief run
  *
  * @todo: document this function
  */
int OpenGLTileEditor::run(Gtk::Window* parent)
{
    if(!dialog_) {
        create_dialog();
    }

    //gtk_level_name_->set_text(""); //Clear the level name on load

    if(parent) {
        dialog_->set_transient_for(*parent);
    }

    int result = dialog_->run();
    dialog_->hide();

    delete dialog_;
    dialog_ = NULL;

    //level_name_ = gtk_level_name_->get_text().raw();
    //tileset_path_ = gtk_directory_->get_text().raw();
    return result;
}

/** @brief OpenGLTileEditorCanvas
  *
  * @todo: document this function
  */
OpenGLTileEditorCanvas::OpenGLTileEditorCanvas(Gtk::DrawingArea* area, OpenGLTileEditor* parent):
OpenGLWidget(area),
parent_(parent),
tile_texture_(0),
drawing_(false)
{
    picker_.reset(new OpenGLPicker<GeometryElement::ptr>());
}

/** @brief do_render
  *
  * @todo: document this function
  */
void OpenGLTileEditorCanvas::do_render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);

    Tile* t = parent_->get_tile();

    if(!t) {
        return;
    }

    if(tile_texture_ == 0) {
        glGenTextures(1, &tile_texture_);
        glBindTexture(GL_TEXTURE_2D, tile_texture_);
        glTexImage2D(GL_TEXTURE_2D, 0, (t->get_channels() == 4) ? GL_RGBA : GL_RGB, t->get_width(), t->get_height(), 0, (t->get_channels() == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, t->get_data());
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glBindTexture(GL_TEXTURE_2D, tile_texture_);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(0, 0);
        glTexCoord2f(1, 0); glVertex2f(t->get_width(), 0);
        glTexCoord2f(1, 1); glVertex2f(t->get_width(), t->get_height());
        glTexCoord2f(0, 1); glVertex2f(0, t->get_height());
    glEnd();

    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 0.0f, 0.0f, 0.9f);

        for(int i = 0; i < t->get_geometry_element_count(); ++i) {
            if(t->get_geometry_element_at(i)->get_layer() != parent_->get_geom_layer_mode()) {
                continue;
            }

            t->get_geometry_element_at(i)->render_geometry();
        }

        current_element_.render_geometry();
    glPopAttrib();
}

/** @brief do_resize
  *
  * @todo: document this function
  */
void OpenGLTileEditorCanvas::do_resize(int width, int height)
{
    glViewport(0, 0, GLfloat(width), GLfloat(height));

    Tile* t = parent_->get_tile();
    if(t) {
        int max = t->get_width();
        if(t->get_height() > max) {
            max = t->get_height();
        }

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-2, max+2, -2, max+2, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
}

/** @brief get_element_from_draw_mode
  *
  * @todo: document this function
  */
GeometryElement OpenGLTileEditorCanvas::get_element_from_draw_mode(const kmVec2& v1, const kmVec2& v2)
{
    if(parent_->get_geom_draw_mode() == GDM_TRIANGLE) {
        return GeometryElement::create_triangle_from(v1, v2);
    } else if (parent_->get_geom_draw_mode() == GDM_CURVE) {
        return GeometryElement::create_curve_from(v1, v2);
    }

    return GeometryElement::create_box_from(v1, v2);
}

/** @brief do_motion
  *
  * @todo: document this function
  */
void OpenGLTileEditorCanvas::do_motion(GdkEventMotion* event)
{
    if(drawing_) {
        kmVec2 pos = unproject(event->x, event->y);
        v2 = pos;

        current_element_ = get_element_from_draw_mode(v1, v2);
    }
}

/** @brief do_button_press
  *
  * @todo: document this function
  */
void OpenGLTileEditorCanvas::do_button_press(GdkEventButton* event)
{
    if(event->button == 1) {
        kmVec2 pos = unproject(event->x, event->y);
        v1 = v2 = pos;

        current_element_ = get_element_from_draw_mode(v1, v2);

        drawing_ = true;
    } else if (event->button == 3) {
        //TODO: Only the current layer iterators should be passed to pick() otherwise we'll get
        //false positives
        MakeCurrent context(this);

        Tile::GeometryIteratorPair iters = parent_->get_tile()->get_geometry_iterators();
        GeometryElement::ptr elem = picker_->pick(event->x, event->y, iters.first, iters.second);
        if(elem && elem->get_layer() == parent_->get_geom_layer_mode()) {
            parent_->get_tile()->delete_geometry_element(elem.get());
            current_element_ = GeometryElement();
        }
    }
}

/** @brief do_button_release
  *
  * @todo: document this function
  */
void OpenGLTileEditorCanvas::do_button_release(GdkEventButton* event)
{
    if(!drawing_) {
        return;
    }

    if(event->button == 1) {
        kmVec2 pos = unproject(event->x, event->y);
        v2 = pos;

        //TODO: if the length of v2-v1 is less than a certain value, don't create the element

        current_element_ = get_element_from_draw_mode(v1, v2);
        current_element_.set_layer(parent_->get_geom_layer_mode());

        parent_->get_tile()->add_geometry_element(GeometryElement::ptr(new GeometryElement(current_element_)));
        current_element_ = GeometryElement();
        drawing_ = false;
    }
}





