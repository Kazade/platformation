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


#ifndef TILE_SELECTOR_H_INCLUDED
#define TILE_SELECTOR_H_INCLUDED

#include <boost/shared_ptr.hpp>
#include <gtkmm.h>
#include <clutter/clutter.h>
#include <clutter-gtk/clutter-gtk.h>
#include "tile.h"

class TileSelector {
public:
    typedef boost::shared_ptr<TileSelector> ptr;

    TileSelector(Gtk::Box* container);

    bool initialize();
    void on_stage_clicked(ClutterEvent* event);

    static void on_stage_clicked_cb(ClutterStage *stage, ClutterEvent *event, gpointer data) {
        TileSelector* _this = (TileSelector*) data;
        _this->on_stage_clicked(event);
    }

    Tile::id_type get_active_tile_id() const { return active_tile_; }

    ClutterStage* get_stage() const { return stage_; }

private:
    Tile::id_type active_tile_;
    Gtk::Box* container_;
    ClutterStage* stage_;
    Gtk::Widget* gtk_tile_canvas_;
};

#endif // TILE_SELECTOR_H_INCLUDED
