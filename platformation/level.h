#ifndef HEADER_14EA6F342AC2C52C
#define HEADER_14EA6F342AC2C52C

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


#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include <GL/gl.h>
#include <cstdint>
#include <set>
#include <vector>
#include <utility>
#include <list>
#include <boost/shared_ptr.hpp>

#include <sigc++/sigc++.h>

#include "kazbase/list_utils.h"

#include "layer.h"
#include "tile_instance.h"
#include "tileset.h"
#include "typedefs.h"

//#include "gimmick_instance.h"
//#include "entity_instance.h"

class Tileset;
class EditorView;

class Level {
public:
    typedef std::tr1::shared_ptr<Level> ptr;

    typedef std::list<TileInstance::ptr> TileList;
    typedef std::pair<TileList::iterator, TileList::iterator> TileListIteratorPair;

    Level(const std::string& name, const uint32_t tile_size);
	void set_dimensions(uint32_t tiles_across, uint32_t tiles_down);

    LayerID create_new_layer();
    void destroy_layer(LayerID layer);
    uint32_t layer_count() const;
    LayerID layer_by_index(uint32_t i) {
        assert(i < layers_.size());
        return layers_.at(i)->id();
    }
    
    uint32_t layer_index(LayerID layer_id);
    
    LayerID layer_by_name(const std::string& name);
    void set_layer_name(LayerID layer_id, std::string new_name) {
        layer(layer_id).set_name(new_name);
    }
    std::string layer_name(LayerID layer_id) { return layer(layer_id).name(); }
    void set_active_layer(LayerID layer_id) { active_layer_ = layer_id; }
    void set_active_layer_by_index(uint32_t index) {
        set_active_layer(layers_.at(index)->id());
    }
    
    void render_layer(EditorView& view, LayerID layer_id);
    
    LayerID active_layer_id() const { return active_layer_; }
    void raise_layer(LayerID l);
    void lower_layer(LayerID l);
    
    bool save(const std::string& filename) const;
    bool load(const std::string& filename);

    void set_level_size(const std::pair<int, int> level_size) {
        signal_changed_();
        level_size_ = level_size;
    }

    std::pair<int, int> get_level_size() const {
        return level_size_;
    }

    sigc::signal<void, Layer*>& layer_created() { return signal_layer_created_; }
    sigc::signal<void, Layer*>& layer_destroyed() { return signal_layer_destroyed_; }
    sigc::signal<void>& signal_changed() { return signal_changed_; }
    sigc::signal<void>& signal_saved() { return signal_saved_; }
    
    void trigger_layer_created_on_all_layers() {
        for(Layer::ptr layer: layers_) {
            signal_layer_created_(layer.get());
        }
    }


    //If a layer is changed, then the level is classed as altered
    void on_layer_changed() { signal_changed_(); }
    
    TileInstance* spawn_tile_instance(Tile::ptr tile) {
        TileID id;
        if(!container::contains(active_tiles_, tile->absolute_path())) {
            //If we haven't used this tile yet
            id = generate_tile_id();
            active_tiles_[tile->absolute_path()] = id;
            tiles_[id] = tile;
            active_tile_refcount_[id] = 1;
        } else {
            id = active_tiles_[tile->absolute_path()];
            active_tile_refcount_[id]++;
        }
        
        TileInstance* instance = active_layer().spawn_tile_instance(id);
        return instance;
    }
    
    void delete_tile_instance(TileInstance* instance) {
        TileID id = instance->tile_id();
        Layer& layer = instance->layer();
        
        active_tile_refcount_[id]--;
        if(!active_tile_refcount_[id]) {
            active_tile_refcount_.erase(id);
            active_tiles_.erase(tiles_[id]->absolute_path());
            tiles_.erase(id);
        }
        
        layer.delete_tile_instance(instance);
    }

    Tile& tile(TileID tile_id) { 
        assert(container::contains(tiles_, tile_id));
        return *tiles_[tile_id];
    }

    TileID generate_tile_id() const {
        static TileID counter = 0;
        return ++counter;
    }

private:
	std::string name_;	
	uint32_t tile_size_;
	
    std::map<std::string, TileID> active_tiles_;
    std::map<TileID, Tile::ptr> tiles_;
    std::map<TileID, uint32_t> active_tile_refcount_;
    
    sigc::signal<void> signal_changed_;
    sigc::signal<void> signal_saved_;
    sigc::signal<void, Layer*> signal_layer_created_;
    sigc::signal<void, Layer*> signal_layer_destroyed_;

    typedef std::vector<Layer::ptr> LayerArray;
    LayerArray layers_;

    std::pair<int, int> level_size_;
    uint32_t active_layer_;
    
    Layer& layer(LayerID layer_id) { 
        for(Layer::ptr layer: layers_) {
            if(layer_id == layer->id()) {
                return *layer;
            }
        }
        
        throw DoesNotExist<Layer>();
    }
    Layer& active_layer() { return layer(layer_by_index(active_layer_)); }    
    
    friend class EditorView; //GAAAAHHHHHH
};


#endif // LEVEL_H_INCLUDED

#endif // header guard
