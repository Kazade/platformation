#include <iostream>
#include "opengl_tile_selector.h"

#include "tileset.h"

float tile_size = 1.0f;
float tile_spacing = 0.25f;

OpenGLTileSelector::OpenGLTileSelector(Gtk::DrawingArea* canvas):
OpenGLWidget(canvas),
tileset_(NULL),
active_tile_(-1),
gtk_tile_edit_menu_(NULL) {
    initialize();
}

/** @brief do_resize
  *
  * @todo: document this function
  */
void OpenGLTileSelector::do_resize(int width, int height)
{
    glClearColor(0.8f, 0.8f, 1.0f, 1.0f);

    float num_tiles_visible = 5.0f;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, width, height);

    float ortho_height = num_tiles_visible * (tile_size + tile_spacing);


    float w = float(width);
    float h = float(height);

    float ratio = h / w;
    h = 2.0f * ratio;

    glOrtho(-1.0f, 1.0f, -h, 0.0f, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/** @brief do_render
  *
  * @todo: document this function
  */
void OpenGLTileSelector::do_render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if(!tileset_) {
        return;
    }

    glEnable(GL_TEXTURE_2D);

    float ytrans = 0.0f;
    if(Gtk::ScrolledWindow* scr = dynamic_cast<Gtk::ScrolledWindow*>(get_widget()->get_parent())) {
        ytrans = scr->get_vadjustment()->get_value();
    }

    glTranslatef(0.0f, ytrans, 0.0f);

    for(int i = 0; i < tileset_->get_tile_count(); ++i) {
        Tile* t = tileset_->get_tile_at(i);
        glBindTexture(GL_TEXTURE_2D, get_texture_for_tile(t));
        t->render_geometry();

        if(active_tile_ == t->get_id()) {
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, -0.1f);
            glLineWidth(2);
            glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_POLYGON_BIT);
                glColor3f(1.0f, 0.0f, 0.0f);
                glDisable(GL_TEXTURE_2D);
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                t->render_geometry();
            glPopAttrib();
            glPopMatrix();
        }
    }
}

/** @brief initialize
  *
  * @todo: document this function
  */
bool OpenGLTileSelector::initialize()
{
    picker_.reset(new OpenGLPicker<Tile::ptr>());

    gtk_tile_edit_menu_ = new Gtk::Menu();
    gtk_edit_tile_item_ = new Gtk::MenuItem("Edit");
    gtk_edit_tile_item_->signal_activate().connect(sigc::mem_fun(this, &OpenGLTileSelector::on_tile_edit));

    gtk_tile_edit_menu_->add(*gtk_edit_tile_item_);
    gtk_tile_edit_menu_->show_all();
    return true;
}

/** @brief generate_texture
  *
  * @todo: document this function
  */
GLuint OpenGLTileSelector::generate_texture(int width, int height, int channels, unsigned char* data)
{
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, (channels == 4) ? GL_RGBA : GL_RGB, width, height, 0, (channels == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    assert(tex);

    return tex;
}


/** @brief get_texture_for_tile
  *
  * @todo: document this function
  */
GLuint OpenGLTileSelector::get_texture_for_tile(Tile* tile)
{
    if(tile_textures_.find(tile->get_id()) != tile_textures_.end()) {
        return tile_textures_[tile->get_id()];
    }

    GLuint tex_id = generate_texture(tile->get_width(), tile->get_height(), tile->get_channels(), tile->get_data());
    tile_textures_[tile->get_id()] = tex_id;
    return tex_id;
}

/** @brief on_button_press
  *
  * @todo: document this function
  */
void OpenGLTileSelector::do_button_press(GdkEventButton* event)
{
    if(event->type == GDK_BUTTON_PRESS && event->button == 3 && active_tile_ != -1) {
        gtk_tile_edit_menu_->popup(event->button, event->time);
        return;
    }

    if(event->type == GDK_BUTTON_PRESS && event->button == 1 && tileset_) {
        MakeCurrent context(this);
        if(!context.ok) {
            return;
        }

        assert(picker_);

        int tile_count = tileset_->get_tile_count();
        if(tile_count == 0) {
            return;
        }

        gfloat x = event->x;
        gfloat y = event->y;

        Tileset::IteratorPair iterators = tileset_->get_iterators();

        Tile::ptr t = picker_->pick(x, y, iterators.first, iterators.second);
        if(t) {
            active_tile_ = t->get_id();
        } else {
            active_tile_ = -1;
        }
    }
}

/** @brief set_tileset
  *
  * @todo: document this function
  */
void OpenGLTileSelector::set_tileset(Tileset* tileset)
{
    tileset_ = tileset;

    float y = -0.75f;

    //todo: fix this, it should get the rendered height from the tile (depending on the ratio)
    Tileset::IteratorPair iterators = tileset->get_iterators();
    for(; iterators.first != iterators.second; ++iterators.first) {
        (*iterators.first)->set_position(0.0f, y);
        y -= 1.25f;
    }

}

/** @brief on_tile_edit
  *
  * @todo: document this function
  */
void OpenGLTileSelector::on_tile_edit()
{
    if(active_tile_ == -1) {
        return;
    }

    tile_editor_.reset(new OpenGLTileEditor(tileset_->get_tile_by_id(active_tile_)));

    if(tile_editor_->run(dynamic_cast<Gtk::Window*>(get_widget()->get_toplevel())) == Gtk::RESPONSE_OK) {
        //tile->save();
    }
}

