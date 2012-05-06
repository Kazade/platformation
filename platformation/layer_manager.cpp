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


#include <boost/bind.hpp>

#include "kazbase/logging/logging.h"
#include "main_window.h"
#include "layer.h"
#include "level.h"
#include "layer_manager.h"
#include "layer_rename_dialog.h"
#include "actions/layer_rename_action.h"
#include "actions/layer_raise_action.h"
#include "actions/layer_lower_action.h"

using boost::bind;

LayerManager::LayerManager(MainWindow* parent, Gtk::TreeView* view, Gtk::Button* add_layer_button, Gtk::Button* delete_layer_button):
    parent_(parent),
    view_(view),
    add_button_(add_layer_button),
    delete_button_(delete_layer_button),
    level_(NULL) {

    tree_model_ = Gtk::TreeStore::create(columns_);
    view_->set_model(tree_model_);
    view_->append_column("Name", columns_.column_name_);
    view_->append_column("Active?", columns_.checked_);

    view_->signal_button_press_event().connect(sigc::mem_fun(*this, &LayerManager::on_layer_popup), false);

    Glib::RefPtr<Gtk::TreeSelection> selection = view_->get_selection();
    selection->set_mode (Gtk::SELECTION_SINGLE);
    selection->signal_changed().connect(sigc::mem_fun(*this, &LayerManager::on_layer_manager_row_activate));

    assert(add_layer_button);
    assert(delete_layer_button);

    add_layer_button->signal_clicked().connect(sigc::mem_fun(*this, &LayerManager::on_add_layer_clicked));
    delete_layer_button->signal_clicked().connect(sigc::mem_fun(*this, &LayerManager::on_delete_layer_clicked));

    gtk_layer_menu_ = manage(new Gtk::Menu());
    gtk_rename_menu_item_ = manage(new Gtk::MenuItem("Rename..."));
    gtk_move_down_menu_item_ = manage(new Gtk::MenuItem("Lower"));
    gtk_move_up_menu_item_ = manage(new Gtk::MenuItem("Raise"));

    gtk_rename_menu_item_->signal_activate().connect(sigc::mem_fun(this, &LayerManager::on_layer_rename));
    gtk_move_up_menu_item_->signal_activate().connect(sigc::mem_fun(this, &LayerManager::on_layer_raise));
    gtk_move_down_menu_item_->signal_activate().connect(sigc::mem_fun(this, &LayerManager::on_layer_lower));
    //TODO: Connect the other menu signals

    gtk_layer_menu_->add(*gtk_rename_menu_item_);
    gtk_layer_menu_->add(*gtk_move_down_menu_item_);
    gtk_layer_menu_->add(*gtk_move_up_menu_item_);
    gtk_layer_menu_->show_all();
}

bool LayerManager::on_layer_popup(GdkEventButton* event) {
    //Popup the layer context menu if this is the right mouse button
    if(event->type == GDK_BUTTON_PRESS && event->button == 3) {
        gtk_layer_menu_->popup(event->button, event->time);
        return true;
    }

    return false;
}

void LayerManager::on_layer_lower() {
    if(!level_->can_lower_layer()) return;
    
    Action::ptr lower_action(new LayerLowerAction(level_, level_->active_layer_id()));
    lower_action->do_action();
    get_main_window()->get_action_manager().push_action(lower_action);
}

void LayerManager::on_layer_raise() {
    if(!level_->can_raise_layer()) return;
    
    Action::ptr raise_action(new LayerRaiseAction(level_, level_->active_layer_id()));
    raise_action->do_action();
    get_main_window()->get_action_manager().push_action(raise_action);
}

void LayerManager::on_layer_rename() {
    LayerRenameDialog::ptr dialog(new LayerRenameDialog());
    int result = dialog->run_dialog();

    if(result == Gtk::RESPONSE_OK) {
        L_DEBUG("Renaming layer");

        std::string new_name = dialog->get_layer_name();

        Level* level = get_main_window()->get_level();
        assert(level);

        std::string old_name = level->layer_name(level_->active_layer_id());

        Action::ptr rename_action(new LayerRenameAction(level, old_name, new_name));
        rename_action->do_action();

        get_main_window()->get_action_manager().push_action(rename_action);
    }
}

void LayerManager::on_layer_created(Layer* layer) {
    update_list_view();
    layer->signal_changed().connect(sigc::mem_fun(this, &LayerManager::update_list_view));
}

void LayerManager::on_layer_destroyed(Layer* layer){
    update_list_view();
}

void LayerManager::set_level(Level* level) {
    level_ = level;

    if(level_) {
        level_->layer_created().connect(bind(&LayerManager::on_layer_created, this, _1));
        level_->layer_destroyed().connect(bind(&LayerManager::on_layer_destroyed, this, _1));
        level_->layers_changed().connect(bind(&LayerManager::update_list_view, this));

        level_->trigger_layer_created_on_all_layers();
    }
}

void LayerManager::update_list_view() {
    L_DEBUG("Updating layer list view");
    
    if(!level_) {
        return;
    }

    tree_model_->clear();

    for(int32_t i = (int32_t) level_->layer_count() - 1; i >= 0; --i) {
        LayerID layer_id = level_->layer_by_index(i);

        Gtk::TreeModel::Row row = *(tree_model_->append());
        row[columns_.column_id_] = i;
        row[columns_.column_name_] = level_->layer_name(layer_id);

        //If this is the active layer, display a tick next to it
        if(layer_id == level_->active_layer_id()) {
            row[columns_.checked_] = view_->render_icon(Gtk::Stock::YES, Gtk::ICON_SIZE_MENU);
        } else {
            row[columns_.checked_] = Glib::RefPtr<Gdk::Pixbuf>();
        }
    }
}

void LayerManager::on_layer_manager_row_activate() {
    if(!level_) {
        return;
    }

    Glib::RefPtr<Gtk::TreeSelection> selection = view_->get_selection();
    Gtk::TreeModel::iterator iter = selection->get_selected();

    if(iter) {
        Gtk::TreeModel::Row row = *iter;
        uint32_t active_layer = row[columns_.column_id_];
        level_->set_active_layer_by_index(active_layer);
    }

    /*
        Go through the rows of the tree view, set the active one to have a checkmark, and the inactive
        ones to have no image
    */
    for(Gtk::TreeModel::iterator it = tree_model_->children().begin(); it != tree_model_->children().end(); ++it) {
        Gtk::TreeModel::Row row = *it;
        if(row[columns_.column_id_] != (*iter)[columns_.column_id_]) {
            row[columns_.checked_] = Glib::RefPtr<Gdk::Pixbuf>();
        } else {
            row[columns_.checked_] = view_->render_icon(Gtk::Stock::YES, Gtk::ICON_SIZE_MENU);
        }
    }
}

void LayerManager::on_add_layer_clicked() {
    if(level_) {
        level_->create_new_layer();
    }
}

void LayerManager::on_delete_layer_clicked() {
    if(level_) {
        level_->destroy_layer(level_->active_layer_id());
    }
}

