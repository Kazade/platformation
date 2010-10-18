#ifndef MAIN_WINDOW_H_INCLUDED
#define MAIN_WINDOW_H_INCLUDED

#include <gtkmm.h>
#include <glibmm.h>
#include <clutter/clutter.h>
#include <clutter-gtk/clutter-gtk.h>

class MainWindow {
public:
    MainWindow();
    void create_widgets();

    Gtk::Window& get_window_ref() { return *gtk_window_; }

private:
    Gtk::Window* gtk_window_;

    Gtk::VBox* gtk_tile_vbox_;
    Gtk::Widget* gtk_tile_canvas_;
    ClutterActor* gtk_tile_stage_;

    void initialize_tile_canvas();
};

#endif // MAIN_WINDOW_H_INCLUDED
