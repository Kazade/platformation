#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include <vector>
#include <utility>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/signals.hpp>

#include "layer.h"
#include "tile_instance.h"
//#include "gimmick_instance.h"
//#include "entity_instance.h"

class Tileset;

class Level {
public:
    typedef boost::shared_ptr<Level> ptr;

    typedef std::list<TileInstance::ptr> TileList;
    typedef std::pair<TileList::iterator, TileList::iterator> TileListIteratorPair;

    typedef boost::signal<void (Layer*)> LayerCreatedSignal;
    typedef boost::signal<void (Layer*)> LayerDestroyedSignal;

    Level(Tileset* tileset);

    //SceneObject* get_selected_object() const;

   // TileInstance* spawn_tile_instance(int tile_id, bool select=true);
    /*GimmickInstance* spawn_gimmick_instance(int gimmick_id);
    EntityInstance* spawn_entity_instance(int entity_id);*/

    Layer* create_new_layer();
    void destroy_layer(Layer* layer);
    uint32_t get_layer_count() const;
    Layer* get_layer_at(uint32_t i);

/*    int get_tile_instance_count() const;
    TileInstance* get_tile_instance_at(int i) const;
    void delete_tile_instance(TileInstance* instance);*/

    /*int get_entity_instance_count() const;
    EntityInstance* get_entity_instance_at(int i) const;

    int get_gimmick_instance_count() const;
    GimmickInstance* get_gimmick_instance_at(int i) const;*/

    bool save(const std::string& filename) const;
    bool load(const std::string& filename);

/*    TileListIteratorPair get_iterators() {
        return std::make_pair(tile_instances_.begin(), tile_instances_.end());
    }*/

    void set_level_size(const std::pair<int, int> level_size) {
        level_size_ = level_size;
    }

    std::pair<int, int> get_level_size() const {
        return level_size_;
    }

    LayerCreatedSignal& layer_created() { return layer_created_; }
    LayerDestroyedSignal& layer_destroyed() { return layer_destroyed_; }

private:
    std::vector<Layer::ptr> layers_;

    /*TileList tile_instances_; */
    Tileset* tileset_;

    std::pair<int, int> level_size_;

    LayerCreatedSignal layer_created_;
    LayerDestroyedSignal layer_destroyed_;
};


#endif // LEVEL_H_INCLUDED
