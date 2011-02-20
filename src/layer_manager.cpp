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

#include "layer.h"
#include "level.h"
#include "layer_manager.h"

using boost::bind;

LayerManager::LayerManager(Gtk::TreeView* view, Gtk::Button* add_layer_button, Gtk::Button* delete_layer_button):
view_(view),
add_button_(add_layer_button),
delete_button_(delete_layer_button),
level_(NULL) {
    tree_model_ = Gtk::TreeStore::create(columns_);
    view_->set_model(tree_model_);
    view_->append_column("Name", columns_.column_name_);

    Glib::RefPtr<Gtk::TreeSelection> selection = view_->get_selection();
    selection->set_mode (Gtk::SELECTION_SINGLE);
    selection->signal_changed().connect(sigc::mem_fun(*this, &LayerManager::on_layer_manager_row_activate));

    assert(add_layer_button);
    assert(delete_layer_button);

    add_layer_button->signal_clicked().connect(sigc::mem_fun(*this, &LayerManager::on_add_layer_clicked));
    delete_layer_button->signal_clicked().connect(sigc::mem_fun(*this, &LayerManager::on_delete_layer_clicked));
}

void LayerManager::on_layer_created(Layer* layer) {
    update_list_view();
}

void LayerManager::on_layer_destroyed(Layer* layer){
    update_list_view();
}

void LayerManager::set_level(Level* level) {
    if(level_) {
        assert(0);
        //FIXME: Disconnect the signals
    }

    level_ = level;

    if(level_) {
        level_->layer_created().connect(bind(&LayerManager::on_layer_created, this, _1));
        level_->layer_destroyed().connect(bind(&LayerManager::on_layer_destroyed, this, _1));

        //Call the callback manually on any already existing layers
        for(uint32_t i = 0; i < level_->get_layer_count(); ++i) {
            on_layer_created(level_->get_layer_at(i));
        }
    }
}

void LayerManager::update_list_view() {
    if(!level_) {
        return;
    }

    tree_model_->clear();

    for(uint32_t i = 0; i < level_->get_layer_count(); ++i) {
        Layer* l = level_->get_layer_at(i);

        Gtk::TreeModel::Row row = *(tree_model_->append());
        row[columns_.column_id_] = i;
        row[columns_.column_name_] = l->get_name();
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
        level_->set_active_layer(active_layer);
    }
}

void LayerManager::on_add_layer_clicked() {
    if(level_) {
        level_->create_new_layer();
    }
}

void LayerManager::on_delete_layer_clicked() {
    if(level_) {
        level_->destroy_layer(level_->get_active_layer());
    }
}

