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
#include <tinyxml.h>
#include <boost/lexical_cast.hpp>

#include "kazbase/logging/logging.h"
#include "level.h"
#include "tileset.h"
#include "editor_view.h"

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
/*    TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

    TiXmlElement* root = new TiXmlElement("level");
    doc.LinkEndChild(root);

    TiXmlElement* tileset = new TiXmlElement("tileset");
    tileset->SetAttribute("path", tileset_->get_path());
    root->LinkEndChild(tileset);

    doc.SaveFile(filename.c_str());*/
    return true;
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
Level::Level(const std::string& name, const uint32_t tile_size):
name_(name),
tile_size_(tile_size),
active_layer_(0) {
    create_new_layer();
}

void Level::set_dimensions(uint32_t tiles_across, uint32_t tiles_down) {
	level_size_.first = tiles_across;
	level_size_.second = tiles_down;
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

LayerID Level::create_new_layer() {
    static int layer_counter = 0;
    
    L_DEBUG("Creating a new layer");

    Layer::ptr new_layer(new Layer(this, "Untitled"));

    for(LayerArray::iterator it = layers_.begin(); it != layers_.end(); ++it) {
        if((*it)->name() == new_layer->name()) {
            new_layer->set_name("Untitled " + boost::lexical_cast<std::string>(++layer_counter));
            break;
        }
    }

    layers_.push_back(new_layer);
    active_layer_ = layer(layer_by_index(layers_.size() - 1)).id();

    //Call signal
    signal_layer_created_(new_layer.get());
    signal_changed_();
    signal_layers_changed_();

    //We want layer changes to trigger level changed signals
    new_layer->signal_changed().connect(sigc::mem_fun(this, &Level::on_layer_changed));
    return new_layer->id();
}

void Level::destroy_layer(LayerID layer_id) {
    L_DEBUG("Destroying layer");

    LayerArray::iterator it = layers_.begin();
    for(; it != layers_.end(); ++it) {
        if((*it)->id() == layer_id) {
            break;
        }
    }

    //If we couldn't find the layer, do nothing
    if(it == layers_.end()) {
        L_WARN("Couldn't find the layer to delete");
        return;
    }

    layers_.erase(it);
    
    L_DEBUG("Layer destroyed");

    if(layers_.empty()) {
        //There must always be one layer, so if it's the last one create a new empty one
        create_new_layer();
        L_DEBUG("New layer created, final one was destroyed");
    }

    active_layer_ = layer(layer_by_index(0)).id();

    signal_changed_();
    signal_layer_destroyed_(NULL); //FIXME: Argument is useless
    signal_layers_changed_();
}

uint32_t Level::layer_count() const {
    return layers_.size();
}

LayerID Level::layer_by_name(const std::string& name) {
    for(Layer::ptr layer: layers_) {
        if(layer->name() == name) {
            return layer->id();
        }
    }
    
    throw DoesNotExist<Layer>();
}

void Level::lower_layer(LayerID layer_id) {
    uint32_t i = layer_index(layer_id);
    if(i > 0) {
        L_DEBUG("Lowering layer");
        //We can swap
        std::swap(layers_[i], layers_[i-1]);
        active_layer_ = layer(layer_by_index(i-1)).id();

        signal_changed_();
        signal_layers_changed_();
    }
}

void Level::raise_layer(LayerID layer_id) {
    uint32_t i = layer_index(layer_id);
    if(i < layers_.size() - 1) {
        L_DEBUG("Raising layer");
        //We can swap
        std::swap(layers_[i], layers_[i+1]);
        active_layer_ = layer(layer_by_index(i+1)).id();

        signal_changed_();
        signal_layers_changed_();
    }
}

void Level::render_layer(EditorView& view, LayerID layer_id) {        
    Layer::TileListIteratorPair iters = layer(layer_id).get_iterators();

    for(; iters.first != iters.second; iters.first++) {
        Object* obj = (*iters.first).get();
        glBindTexture(GL_TEXTURE_2D, view.get_texture_for_object(obj));
        obj->render_geometry();
    }        
}

uint32_t Level::layer_index(LayerID layer_id) const { 
    uint32_t idx = 0;
    for(Layer::ptr layer: layers_) {
        if(layer->id() == layer_id) {
            return idx;
        }
        idx++;
    }
    throw DoesNotExist<Layer>();
}
