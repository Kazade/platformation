#include "spawn_tile_instance_action.h"

#include "../opengl_tile_selector.h"

SpawnTileInstanceAction::SpawnTileInstanceAction(Level* level, OpenGLTileSelector* selector, float x, float y):
    Action(level),
    selector_(selector),
    spawned_instance_(NULL),
    x_(x),
    y_(y) {

}

void SpawnTileInstanceAction::do_action() {
    Layer* layer = level_->get_active_layer();
    Tile::id_type id = selector_->get_active_tile_id();
    if(id != -1) {
        spawned_instance_ = layer->spawn_tile_instance(id);
        spawned_instance_->set_position(x_, y_);
    }
}

void SpawnTileInstanceAction::undo_action() {
    assert(0 && "Undo not implemented");
}

std::string SpawnTileInstanceAction::get_description() {
    return "create tile";
}
