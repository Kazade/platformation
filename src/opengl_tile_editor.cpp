#include "opengl_tile_editor.h"

#include "tile.h"

#define UI_FILE "ui/tile_editor.glade"

/** @brief OpenGLTileEditor
  *
  * @todo: document this function
  */
OpenGLTileEditor::OpenGLTileEditor(Tile* tile):
tile_(tile)
{
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
    builder->add_from_file(UI_FILE);

    builder->get_widget("tile_editor_dialog", dialog_);
    assert(dialog_);

    builder->get_widget("tile_editor_canvas", tile_editor_canvas_);
    assert(tile_editor_canvas_);

    canvas_.reset(new OpenGLTileEditorCanvas(tile_editor_canvas_, this));
    dialog_->show_all();
}

/** @brief run
  *
  * @todo: document this function
  */
int OpenGLTileEditor::run()
{
    //gtk_level_name_->set_text(""); //Clear the level name on load

    //dialog_->set_transient_for(*parent);

    int result = dialog_->run();
    dialog_->hide();

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
tile_texture_(0)
{

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
        glOrtho(0, max, 0, max, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
}





