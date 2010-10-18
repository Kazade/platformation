#include "main_window.h"

#define UI_FILE "ui/main_window.glade"

MainWindow::MainWindow():
gtk_window_(NULL),
gtk_tile_canvas_(NULL),
gtk_tile_canvas_embed_(NULL) {
    create_widgets();
}

void MainWindow::create_widgets() {
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();

    //Just let exceptions propagate - we can't do anything about them anyway
    builder->add_from_file(UI_FILE);
    builder->get_widget("main_window", gtk_window_);
    builder->get_widget("tile_vbox", gtk_tile_vbox_);

    gtk_window_->show_all();
    initialize_tile_canvas();
}

void MainWindow::initialize_tile_canvas() {
    gtk_tile_canvas_ = new Clutter::Gtk::Embed();
    gtk_tile_vbox_->pack_start(*Gtk::manage(gtk_tile_canvas_), Gtk::PACK_EXPAND_WIDGET);
    gtk_tile_stage_ = gtk_tile_canvas_->get_stage();
    gtk_tile_stage_->reference();
    gtk_tile_stage_->set_color(Clutter::Color(0, 0, 0));
    gtk_tile_stage_->show();
}
