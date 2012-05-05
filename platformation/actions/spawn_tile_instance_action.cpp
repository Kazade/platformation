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
    LayerID layer = level_->active_layer_id();

    //If this is a redo, we will have stored the layer name, so get that
    if(!layer_name_.empty()) {
        layer = level_->layer_by_name(layer_name_);
        //Make sure it's the active layer
        level_->set_active_layer(layer);
    }

    //Store the layer name (on a redo this will be a no-op)
    layer_name_ = level_->layer_name(layer); //For this to work, layer names must be unique, and undo of a layer delete should restore the name

    if(tile_id_ == -1) {
        //This is not a redo (because there is no stored id) so use the active one
        tile_id_ = selector_->get_active_tile_id();
    }

    if(tile_id_ != -1) {
        Tile::ptr tile = selector_->library().tile(tile_id_);
        //Finally, spawn the instance
        spawned_instance_ = level_->spawn_tile_instance(tile);
        spawned_instance_->set_position(x_, y_);
    }
}

void SpawnTileInstanceAction::undo_action() {
    /*
     * Get the layer we are supposed to be deleting an instance from
     * then make sure it's active and delete the instance
     */
    LayerID layer = level_->layer_by_name(layer_name_);
    level_->set_active_layer(layer);
    level_->delete_tile_instance(spawned_instance_);

    //Clear the spawned instance
    spawned_instance_ = NULL;
}

std::string SpawnTileInstanceAction::get_description() {
    return "create tile";
}
