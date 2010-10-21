#include <cassert>
#include <boost/bind.hpp>
#include <iostream>

#include "main_window.h"
#include "new_level_dialog.h"
#include "clutter_helper.h"

#define UI_FILE "ui/main_window.glade"

MainWindow::MainWindow():
gtk_window_(NULL) {
    create_widgets();
    connect_signals();

    selector_.reset(new TileSelector(gtk_tile_vbox_));
}

void MainWindow::create_widgets() {
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();

    //Just let exceptions propagate - we can't do anything about them anyway
    builder->add_from_file(UI_FILE);
    builder->get_widget("main_window", gtk_window_);
    builder->get_widget("tile_vbox", gtk_tile_vbox_);
    builder->get_widget("new_level_item", gtk_new_level_item_);

    gtk_window_->show_all();
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
        tileset_->update_stage(selector_->get_stage());

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

