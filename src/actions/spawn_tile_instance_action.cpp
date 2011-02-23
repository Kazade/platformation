#include "spawn_tile_instance_action.h"

#include "../opengl_tile_selector.h"

SpawnTileInstanceAction::SpawnTileInstanceAction(Level* level, OpenGLTileSelector* selector, float x, float y):
    Action(level),
    selector_(selector),
    spawned_instance_(NULL),
    x_(x),
    y_(y),
    tile_id_(-1) {

}

void SpawnTileInstanceAction::do_action() {
    //If this is not a redo, get the active layer
    Layer* layer = level_->get_active_layer();

    //If this is a redo, we will have stored the layer name, so get that
    if(!layer_name_.empty()) {
        layer = level_->get_layer_by_name(layer_name_);
    }

    assert(layer);

    //Store the layer name (on a redo this will be a no-op)
    layer_name_ = layer->get_name(); //For this to work, layer names must be unique, and undo of a layer delete should restore the name

    if(tile_id_ == -1) {
        //This is not a redo (because there is no stored id) so use the active one
        tile_id_ = selector_->get_active_tile_id();
    }

    if(tile_id_ != -1) {
        //Finally, spawn the instance
        spawned_instance_ = layer->spawn_tile_instance(tile_id_);
        spawned_instance_->set_position(x_, y_);
    }
}

void SpawnTileInstanceAction::undo_action() {
    Layer* layer = level_->get_layer_by_name(layer_name_);
    assert(layer);
    layer->delete_tile_instance(spawned_instance_);

    //Clear the spawned instance
    spawned_instance_ = NULL;
}

std::string SpawnTileInstanceAction::get_description() {
    return "create tile";
}
