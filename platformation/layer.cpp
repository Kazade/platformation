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

#include <boost/format.hpp>

#include "kazbase/logging/logging.h"
#include "layer.h"
#include "level.h"

using namespace std;

Layer::Layer(Level* parent, const std::string& name):
id_(Layer::generate_layer_id()),
level_(parent),
name_(name) {
    std::pair<uint32_t, uint32_t> dimensions = level_->size();
    resize(dimensions.first, dimensions.second);
}

void Layer::resize(uint32_t w, uint32_t h) {
    //FIXME: preserve old tiles if we get larger/smaller
    L_DEBUG("Resizing the layer");
    
    uint32_t new_total = w * h;
    
    if(new_total > tile_instances_.size()) {
        tile_instances_.reserve(new_total);
        uint32_t to_make = (new_total - tile_instances_.size());
        for(uint32_t i = 0; i < to_make; ++i) {
            tile_instances_.push_back(TileInstance::ptr(new TileInstance(this)));
        }
    } else {
        tile_instances_.resize(new_total);
    }
    
    for(uint32_t y = 0; y < h; ++y) {
        for(uint32_t x = 0; x < w; ++x) {
            TileInstance::ptr instance = tile_instances_.at((y * w) + x);
            kglt::Mesh& mesh = level_->scene().mesh(instance->mesh_id());
            mesh.move_to(float(x) + 0.5, float(y) + 0.5, -1.0);
        }
    }
    
    L_DEBUG("Layer resize complete");
}

void Layer::set_name(const std::string& name) {
    name_ = name;
    signal_changed_();
}

string Layer::name() const {
    return name_;
}
