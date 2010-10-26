#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include <utility>
#include <list>
#include <boost/shared_ptr.hpp>

#include "tile_instance.h"
//#include "gimmick_instance.h"
//#include "entity_instance.h"

class Tileset;

class Level {
public:
    typedef boost::shared_ptr<Level> ptr;

    typedef std::list<TileInstance::ptr> ObjectList;
    typedef std::pair<ObjectList::iterator, ObjectList::iterator> ObjectListIteratorPair;

    Level(Tileset* tileset);

    //SceneObject* get_selected_object() const;

    TileInstance* spawn_tile_instance(int tile_id, bool select=true);
    /*GimmickInstance* spawn_gimmick_instance(int gimmick_id);
    EntityInstance* spawn_entity_instance(int entity_id);*/

    int get_tile_instance_count() const;
    TileInstance* get_tile_instance_at(int i) const;

    /*int get_entity_instance_count() const;
    EntityInstance* get_entity_instance_at(int i) const;

    int get_gimmick_instance_count() const;
    GimmickInstance* get_gimmick_instance_at(int i) const;*/

    bool save(const std::string& filename) const;
    bool load(const std::string& filename);

    ObjectListIteratorPair get_iterators() {
        return std::make_pair(tile_instances_.begin(), tile_instances_.end());
    }

private:
    ObjectList tile_instances_;
    Tileset* tileset_;

};


#endif // LEVEL_H_INCLUDED
