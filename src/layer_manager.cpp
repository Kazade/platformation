#include <boost/bind.hpp>

#include "layer.h"
#include "level.h"
#include "layer_manager.h"

using boost::bind;

LayerManager::LayerManager(Gtk::TreeView* view):
view_(view),
level_(NULL) {

}

void LayerManager::on_layer_created(Layer* layer)
{
}

void LayerManager::on_layer_destroyed(Layer* layer)
{
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
