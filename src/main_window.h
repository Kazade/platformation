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


#ifndef MAIN_WINDOW_H_INCLUDED
#define MAIN_WINDOW_H_INCLUDED

#include <gtkmm.h>
#include <glibmm.h>
#include <clutter/clutter.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "level.h"
#include "tileset.h"
#include "opengl_tile_selector.h"
#include "editor_view.h"
#include "layer_manager.h"

class MainWindow {
public:
    MainWindow();
    void create_widgets();

    Gtk::Window& get_window_ref() { return *gtk_window_; }

    void on_new_level_activate();
    void on_add_tile_clicked();

private:
    Gtk::Window* gtk_window_;

    Gtk::VBox* gtk_tile_vbox_;
    Gtk::MenuItem* gtk_new_level_item_;
    Gtk::Button* gtk_add_tile_button_;
    Gtk::Button* gtk_add_layer_button_;
    Gtk::Button* gtk_delete_layer_button_;

    Gtk::DrawingArea* gtk_canvas_, *gtk_tile_selector_canvas_;
    Gtk::TreeView* gtk_layer_view_;

    void initialize_tile_canvas();
    void connect_signals();

    Level::ptr level_;
    Tileset::ptr tileset_;
    OpenGLTileSelector::ptr selector_;
    EditorView::ptr editor_view_;
    LayerManager::ptr layer_manager_;
};

#endif // MAIN_WINDOW_H_INCLUDED
