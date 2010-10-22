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
