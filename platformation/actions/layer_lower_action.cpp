#include "layer_lower_action.h"

LayerLowerAction::LayerLowerAction(Level* level, LayerID layer):
    Action(level),
    layer_id_(layer) {
    
}

void LayerLowerAction::do_action() {
    assert(level_);
    level_->lower_layer(layer_id_);
}

void LayerLowerAction::undo_action() {
    assert(level_);
    level_->raise_layer(layer_id_);
}
