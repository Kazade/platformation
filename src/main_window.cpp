#include <cassert>

#include "main_window.h"
#include "new_level_dialog.h"

#define UI_FILE "ui/main_window.glade"

MainWindow::MainWindow():
gtk_window_(NULL),
gtk_tile_canvas_(NULL),
gtk_tile_stage_(NULL) {
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
    dialog->run_dialog(gtk_window_);
}

/** @brief connect_signals
  *
  * @todo: document this function
  */
void MainWindow::connect_signals() {
    assert(gtk_new_level_item_);
    gtk_new_level_item_->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_new_level_activate));
}

