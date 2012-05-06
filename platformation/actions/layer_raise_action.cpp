#include "layer_raise_action.h"

LayerRaiseAction::LayerRaiseAction(Level* level, LayerID layer):
    Action(level),
    layer_id_(layer) {
    
}

void LayerRaiseAction::do_action() {
    assert(level_);
    level_->raise_layer(layer_id_);
}

void LayerRaiseAction::undo_action() {
    assert(level_);
    level_->lower_layer(layer_id_);
}
