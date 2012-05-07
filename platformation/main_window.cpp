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


#include <cassert>
#include <boost/bind.hpp>
#include <iostream>

#include "kazbase/logging/logging.h"
#include "main_window.h"
#include "new_level_dialog.h"

#define UI_FILE "ui/main_window.glade"

/**
 * TODO:
 * 
 * 1. Move tiles into the level, instead of choosing a folder to work from, import tiles into the project
 * 2. Allow changing the level size in its configuration, default to small
 * 3. Allow changing the transparent colour in the configuration, default to pink or something
 * 4. Make the initial screen start with a blank level with a single layer and no tiles
 * 5. Fix the tile selector so the scrollbar isn't idiotic
 * 6. Move all actions to the action framework
 * 7. ???
 * 8. Profit!
 */

MainWindow::MainWindow():
    gtk_window_(NULL),
    gtk_add_tile_button_(NULL) {
    create_widgets();
    connect_signals();
}

void MainWindow::create_widgets() {
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
    
    Gtk::DrawingArea* canvas = nullptr;

    //Just let exceptions propagate - we can't do anything about them anyway
    builder->add_from_file(UI_FILE);
    builder->get_widget("main_window", gtk_window_);
    builder->get_widget("tile_selector_canvas", gtk_tile_selector_canvas_);
    builder->get_widget("new_toolbutton", gtk_new_toolbutton_);
    builder->get_widget("save_toolbutton", gtk_save_toolbutton_);
    builder->get_widget("undo_toolbutton", gtk_undo_toolbutton_);
    builder->get_widget("redo_toolbutton", gtk_redo_toolbutton_);

    builder->get_widget("add_tile_button", gtk_add_tile_button_);
    builder->get_widget("canvas", canvas);
    builder->get_widget("layer_tree_view", gtk_layer_view_);
    builder->get_widget("add_layer_button", gtk_add_layer_button_);
    builder->get_widget("delete_layer_button", gtk_delete_layer_button_);
    builder->get_widget("side_bar_alignment", gtk_side_bar_);

    gtk_canvas_.reset(new GtkGLWidget(canvas));

    //assert(gtk_canvas_);
    editor_view_.reset(new EditorView(gtk_canvas_->area(), this));

    assert(gtk_tile_selector_canvas_);
    selector_.reset(new OpenGLTileSelector(gtk_tile_selector_canvas_));
    
    if(editor_view_) {
        editor_view_->set_tile_selector(selector_.get());
    }

    layer_manager_.reset(new LayerManager(this, gtk_layer_view_, gtk_add_layer_button_, gtk_delete_layer_button_));

    gtk_window_->show_all();

    gtk_save_toolbutton_->set_sensitive(false);
    gtk_undo_toolbutton_->set_sensitive(false);
    gtk_redo_toolbutton_->set_sensitive(false);

    gtk_undo_toolbutton_->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_undo));
    gtk_redo_toolbutton_->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_redo));
    get_action_manager().signal_changed().connect(sigc::mem_fun(this, &MainWindow::on_action_manager_change));

    gtk_window_->maximize();
    
	create_new_level("Untitled", 128);    
}

void MainWindow::on_undo() {
    get_action_manager().undo();
}

void MainWindow::on_redo() {
    get_action_manager().redo();
}

void MainWindow::on_action_manager_change() {
    if(get_action_manager().can_redo()) {
        gtk_redo_toolbutton_->set_sensitive(true);
    } else {
        gtk_redo_toolbutton_->set_sensitive(false);
    }

    if(get_action_manager().can_undo()) {
        gtk_undo_toolbutton_->set_sensitive(true);
    } else {
        gtk_undo_toolbutton_->set_sensitive(false);
    }
}

void MainWindow::on_level_changed() {
    gtk_save_toolbutton_->set_sensitive(true);
}

void MainWindow::on_level_saved() {
    gtk_save_toolbutton_->set_sensitive(false);
}

void MainWindow::create_new_level(const std::string& name, uint32_t tile_size) {
	L_DEBUG("Creating a level with name: " + name);
	
	level_.reset(new Level(name, tile_size));	
	level_->set_dimensions(20, 6);
	
	if(level_changed_connection_.connected()) {
		level_changed_connection_.disconnect();
	}

	if(level_saved_connection_.connected()) {
		level_saved_connection_.disconnect();
	}

	level_changed_connection_ = level_->signal_changed().connect(sigc::mem_fun(this, &MainWindow::on_level_changed));
	level_saved_connection_ = level_->signal_saved().connect(sigc::mem_fun(this, &MainWindow::on_level_saved));
	
    if(editor_view_) {
        editor_view_->set_level(level_.get());
    }
    
	layer_manager_->set_level(level_.get()); 
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
   /*     L_DEBUG("Creating a new level");

        std::string level_name = dialog->get_level_name();
        std::string tileset_path = dialog->get_tileset_path();
        uint8_t r, g, b;
        dialog->get_transparent_colour(r, g, b);

        tileset_ = Tileset::load_from_directory(tileset_path, TransparentColour(r, g, b));

        selector_->set_tileset(tileset_.get());
        level_.reset(new Level(tileset_.get()));
        level_->set_level_size(dialog->get_level_size());

        if(level_changed_connection_.connected()) {
            level_changed_connection_.disconnect();
        }

        if(level_saved_connection_.connected()) {
            level_saved_connection_.disconnect();
        }

        level_changed_connection_ = level_->signal_changed().connect(sigc::mem_fun(this, &MainWindow::on_level_changed));
        level_saved_connection_ = level_->signal_saved().connect(sigc::mem_fun(this, &MainWindow::on_level_saved));

        editor_view_->set_level(level_.get());
        layer_manager_->set_level(level_.get());        */
    }
}

void MainWindow::on_save_level_activate() {
    if(!level_) {
        L_WARN("Tried to save when there is no level");
        return;
    }

    Gtk::FileChooserDialog dialog("Please choose a filename", Gtk::FILE_CHOOSER_ACTION_SAVE);

    dialog.set_transient_for(get_window_ref());

    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);

    Gtk::FileFilter filter_text;
    filter_text.set_name("Platformation files (*.plfn)");
    filter_text.add_pattern("*.plfn");
    dialog.add_filter(filter_text);

    int result = dialog.run();

    if(result == Gtk::RESPONSE_OK) {
        L_DEBUG("Saving level");
        level_->save(dialog.get_filename()); //TODO: Handle errors
    }
}

void MainWindow::set_side_panel_visible(bool v) {
    gtk_side_bar_->set_visible(v);
}

/** @brief connect_signals
  *
  * @todo: document this function
  */
void MainWindow::connect_signals() {
    assert(gtk_new_toolbutton_);
    assert(gtk_save_toolbutton_);

    gtk_new_toolbutton_->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_new_level_activate));
    gtk_save_toolbutton_->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_save_level_activate));
    gtk_add_tile_button_->signal_clicked().connect(sigc::mem_fun(this, &MainWindow::on_add_tile_clicked));
}

/** @brief on_add_tile_clicked
  *
  * @todo: document this function
  */
void MainWindow::on_add_tile_clicked()
{
    if(!level_ || selector_->get_active_tile_id() == -1) {
        return;
    }

    level_->spawn_tile_instance(selector_->library().tile(selector_->get_active_tile_id()));
}

