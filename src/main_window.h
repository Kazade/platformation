#ifndef MAIN_WINDOW_H_INCLUDED
#define MAIN_WINDOW_H_INCLUDED

#include <gtkmm.h>
#include <glibmm.h>
#include <clutter/clutter.h>
#include <clutter-gtk/clutter-gtk.h>

#include "level.h"
#include "tileset.h"

class MainWindow {
public:
    MainWindow();
    void create_widgets();

    Gtk::Window& get_window_ref() { return *gtk_window_; }

    void on_new_level_activate();

private:
    Gtk::Window* gtk_window_;

    Gtk::VBox* gtk_tile_vbox_;
    Gtk::Widget* gtk_tile_canvas_;
    ClutterActor* gtk_tile_stage_;

    Gtk::MenuItem* gtk_new_level_item_;

    void initialize_tile_canvas();
    void connect_signals();

    Level::ptr level_;
    Tileset::ptr tileset_;
};

#endif // MAIN_WINDOW_H_INCLUDED
