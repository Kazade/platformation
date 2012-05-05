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


#include "layer.h"
#include "level.h"

using namespace std;

Layer::Layer(Level* parent, const std::string& name):
id_(Layer::generate_layer_id()),
level_(parent),
name_(name) {

}

TileInstance* Layer::spawn_tile_instance(TileID tile_id, bool select) {
    TileInstance::ptr new_tile_instance(new TileInstance(this, tile_id));
    tile_instances_.push_back(new_tile_instance);

    signal_changed_();

    return new_tile_instance.get();
}

uint32_t Layer::get_tile_instance_count() const {
    return tile_instances_.size();
}

TileInstance* Layer::get_tile_instance_at(uint32_t i) const {
    assert(0);
    return NULL;
}

void Layer::delete_tile_instance(TileInstance* instance) {
    TileListIteratorPair iters = get_iterators();

    for(; iters.first != iters.second; ++iters.first) {
        TileInstance* rhs = (*iters.first).get();
        if(rhs == instance) {
            break;
        }
    }

    if(iters.first == iters.second) {
        return;
    }
    tile_instances_.erase(iters.first);

    signal_changed_();
}

void Layer::set_name(const std::string& name) {
    name_ = name;
    signal_changed_();
}

string Layer::name() const {
    return name_;
}
