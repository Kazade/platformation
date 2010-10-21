#include <cassert>
#include <boost/bind.hpp>
#include <iostream>

#include "main_window.h"
#include "new_level_dialog.h"
#include "clutter_helper.h"

#define UI_FILE "ui/main_window.glade"

MainWindow::MainWindow():
gtk_window_(NULL),
gtk_tile_canvas_(NULL),
gtk_tile_stage_(NULL),
active_tile_(-1) {
    create_widgets();
    connect_signals();
}

void MainWindow::create_widgets() {
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();

    //Just let exceptions propagate - we can't do anything about them anyway
    builder->add_from_file(UI_FILE);
    builder->get_widget("main_window", gtk_window_);
    builder->get_widget("tile_vbox", gtk_tile_vbox_);
    builder->get_widget("new_level_item", gtk_new_level_item_);

    gtk_window_->show_all();
    initialize_tile_canvas();
}

void MainWindow::initialize_tile_canvas() {
    GtkClutterEmbed* embed = (GtkClutterEmbed*) gtk_clutter_embed_new();
    assert(embed);

    gtk_tile_canvas_ = Glib::wrap((GtkWidget*) embed);
    gtk_tile_vbox_->pack_start(*Gtk::manage(gtk_tile_canvas_), Gtk::PACK_EXPAND_WIDGET);

    gtk_tile_stage_ = gtk_clutter_embed_get_stage(embed);
    assert(gtk_tile_stage_);

    clutter_stage_set_color((ClutterStage*)gtk_tile_stage_, clutter_color_new(255, 255, 255, 0));

    gtk_tile_canvas_->show();
    clutter_actor_show(gtk_tile_stage_);

    //Connect (in a round-about way) the on_tile_stage_clicked method
    g_signal_connect(gtk_tile_stage_, "button-press-event", G_CALLBACK(MainWindow::on_tile_stage_clicked_cb), this);
}

void MainWindow::on_tile_stage_clicked(ClutterEvent* event) {
    gfloat x = 0;
    gfloat y = 0;
    clutter_event_get_coords (event, &x, &y);
    ClutterActor* a = clutter_stage_get_actor_at_pos(CLUTTER_STAGE(gtk_tile_stage_), CLUTTER_PICK_ALL, x, y);
    if(a && a != gtk_tile_stage_) {
        Tile* t = (Tile*) g_object_get_data(G_OBJECT(a), "tile");
        active_tile_ = t->get_id();
        std::cout << "Tile clicked: " << active_tile_ << std::endl;
    } else {
        active_tile_ = -1;
    }
}


/** @brief on_new_level
  *
  * Display a dialog and create a new level if neccessary
  * @todo Ask to save current level if one exists
  * @todo Create the new level
  * @todo Initialize the tileset view
  */
void MainWindow::on_new_level_activate() {
    NewLevelDialog::ptr dialog(new NewLevelDialog());
    int result = dialog->run_dialog(gtk_window_);

    if(result == Gtk::RESPONSE_OK) {
        std::string level_name = dialog->get_level_name();
        std::string tileset_path = dialog->get_tileset_path();

        tileset_ = Tileset::load_from_directory(tileset_path);
        tileset_->update_stage((ClutterStage*)gtk_tile_stage_);

        level_.reset(new Level(tileset_.get()));
    }
}

/** @brief connect_signals
  *
  * @todo: document this function
  */
void MainWindow::connect_signals() {
    assert(gtk_new_level_item_);
    gtk_new_level_item_->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_new_level_activate));
}

