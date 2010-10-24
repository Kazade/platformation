#include <cassert>
#include <boost/bind.hpp>
#include <iostream>

#include "main_window.h"
#include "new_level_dialog.h"
#include "clutter_helper.h"

#define UI_FILE "ui/main_window.glade"

MainWindow::MainWindow():
gtk_window_(NULL),
gtk_add_tile_button_(NULL) {
    create_widgets();
    connect_signals();
}

void MainWindow::create_widgets() {
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();

    //Just let exceptions propagate - we can't do anything about them anyway
    builder->add_from_file(UI_FILE);
    builder->get_widget("main_window", gtk_window_);
    builder->get_widget("tile_selector_canvas", gtk_tile_selector_canvas_);
    builder->get_widget("new_level_item", gtk_new_level_item_);
    builder->get_widget("add_tile_button", gtk_add_tile_button_);
    builder->get_widget("canvas", gtk_canvas_);

    assert(gtk_canvas_);
    editor_view_.reset(new EditorView(gtk_canvas_));

    assert(gtk_tile_selector_canvas_);
    selector_.reset(new OpenGLTileSelector(gtk_tile_selector_canvas_));

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
        selector_->set_tileset(tileset_.get());
        level_.reset(new Level(tileset_.get()));
    }
}

/** @brief connect_signals
  *
  * @todo: document this function
  */
void MainWindow::connect_signals() {
    assert(gtk_new_level_item_);
    assert(gtk_add_tile_button_);

    gtk_new_level_item_->signal_activate().connect(sigc::mem_fun(this, &MainWindow::on_new_level_activate));
    gtk_add_tile_button_->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_add_tile_clicked));
}

/** @brief on_add_tile_clicked
  *
  * @todo: document this function
  */
void MainWindow::on_add_tile_clicked()
{
    if(!level_) {
        return;
    }

    level_->spawn_tile_instance(selector_->get_active_tile_id());
}

