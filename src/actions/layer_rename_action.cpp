#include "layer_rename_action.h"

LayerRenameAction::LayerRenameAction(Level* level, const std::string& old_name, const std::string& new_name):
    Action(level),
    old_name_(old_name),
    new_name_(new_name) {

}

void LayerRenameAction::do_action() {
    Layer* layer = level_->get_layer_by_name(old_name_);
    assert(layer && "Layer not found");

    layer->set_name(new_name_);
}

void LayerRenameAction::undo_action() {
    Layer* layer = level_->get_layer_by_name(new_name_);
    assert(layer && "Layer not found");

    layer->set_name(old_name_);
}

std::string LayerRenameAction::get_description() {
    return "rename layer";
}
