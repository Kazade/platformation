/***********************************************************************************
*
*  This program is free software; you can redistribute it and/or modify 
*  it under the terms of the GNU Lesser General Public License as published 
*  by the Free Software Foundation; either version 3 of the License, or (at 
*  your option) any later version.
*
*  This program is distributed in the hope that it will be useful, but 
*  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
*  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public 
*  License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License 
*  along with this program; if not, see <http://www.gnu.org/copyleft/lesser.html>.
*
**********************************************************************************/


#ifndef OPENGL_TILE_SELECTOR_H_INCLUDED
#define OPENGL_TILE_SELECTOR_H_INCLUDED

#include <boost/shared_ptr.hpp>
#include <gtkmm.h>

#include "opengl_picker.h"
#include "opengl_widget.h"
#include "opengl_tile_editor.h"
#include "tile.h"

class Tileset;

class OpenGLTileSelector : public OpenGLWidget {
public:
    typedef std::tr1::shared_ptr<OpenGLTileSelector> ptr;

    OpenGLTileSelector(Gtk::DrawingArea* canvas);

    bool initialize();

    Tile::id_type get_active_tile_id() const { return active_tile_; }

    void set_tileset(Tileset* tileset);

    void on_tile_edit();

private:
    Tile::id_type active_tile_;

    void do_render();
    void do_resize(int width, int height);
    void do_scroll(GdkEventScroll* event);

    Tileset* tileset_;

    std::map<Tile::id_type, GLuint> tile_textures_;

    GLuint generate_texture(int width, int height, int channels, unsigned char* data);

    GLuint get_texture_for_tile(Tile* tile);

    void do_button_press(GdkEventButton* event);

    OpenGLPicker<Tile::ptr>::ptr picker_;
    Gtk::Menu* gtk_tile_edit_menu_;
    Gtk::MenuItem* gtk_edit_tile_item_;

    OpenGLTileEditor::ptr tile_editor_;

    float total_display_height_;
};

#endif // OPENGL_TILE_SELECTOR_H_INCLUDED
