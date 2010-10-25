#ifndef OPENGL_TILE_SELECTOR_H_INCLUDED
#define OPENGL_TILE_SELECTOR_H_INCLUDED

#include <boost/shared_ptr.hpp>
#include <gtkmm.h>

#include "opengl_picker.h"
#include "opengl_widget.h"
#include "tile.h"

class Tileset;

class OpenGLTileSelector : public OpenGLWidget {
public:
    typedef boost::shared_ptr<OpenGLTileSelector> ptr;

    OpenGLTileSelector(Gtk::DrawingArea* canvas);

    bool initialize();

    Tile::id_type get_active_tile_id() const { return active_tile_; }

    void set_tileset(Tileset* tileset);


private:
    Tile::id_type active_tile_;

    void do_render();
    void do_resize(int width, int height);

    Tileset* tileset_;

    std::map<Tile::id_type, GLuint> tile_textures_;

    GLuint generate_texture(int width, int height, int channels, unsigned char* data);

    GLuint get_texture_for_tile(Tile* tile);

    void do_button_press(GdkEventButton* event);

    OpenGLPicker<Tile::ptr>::ptr picker_;
};

#endif // OPENGL_TILE_SELECTOR_H_INCLUDED
