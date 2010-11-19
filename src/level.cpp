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


#include <cassert>

#include "level.h"

/** @brief load
  *
  * @todo: document this function
  */
bool Level::load(const std::string& filename)
{
    assert(0);
    return false;
}

/** @brief save
  *
  * @todo: document this function
  */
bool Level::save(const std::string& filename) const
{
    assert(0);
    return false;
}

/** @brief get_tile_instance_at
  *
  * @todo: document this function
  */
/*TileInstance* Level::get_tile_instance_at(int i) const
{
    assert(0);
    return NULL;
}*/

/** @brief get_tile_instance_count
  *
  * @todo: document this function
  */
/*int Level::get_tile_instance_count() const
{
    assert(0);
    return 0;
}*/

/** @brief spawn_tile_instance
  *
  * @todo: document this function
  */
/*TileInstance* Level::spawn_tile_instance(int tile_id, bool select)
{
    TileInstance::ptr new_tile_instance(new TileInstance(tileset_, tile_id));
    tile_instances_.push_back(new_tile_instance);
    return new_tile_instance.get();
}*/

/** @brief Level
  *
  * @todo: document this function
  */
Level::Level(Tileset* tileset):
tileset_(tileset),
active_layer_(0)
{
    create_new_layer();
}

/** @brief delete_tile_instance
  *
  * @todo: document this function
  */
/*void Level::delete_tile_instance(TileInstance* instance)
{
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
}*/

Layer* Level::create_new_layer() {
    Layer::ptr new_layer(new Layer(tileset_));
    layers_.push_back(new_layer);
    active_layer_ = layers_.size() - 1;

    //Call signal
    layer_created_(new_layer.get());
    return new_layer.get();
}

void Level::destroy_layer(Layer* layer) {
    assert(0);
    layer_destroyed_(layer);
}

uint32_t Level::get_layer_count() const {
    return layers_.size();
}

Layer* Level::get_layer_at(uint32_t i) {
    assert(i >= 0 && i < layers_.size());
    return layers_[i].get();
}
