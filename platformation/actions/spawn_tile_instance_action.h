#ifndef SPAWN_TILE_INSTANCE_ACTION_H_INCLUDED
#define SPAWN_TILE_INSTANCE_ACTION_H_INCLUDED

#include "action.h"
#include "../tile.h"

class OpenGLTileSelector;

class SpawnTileInstanceAction : public Action {
public:
    typedef boost::shared_ptr<SpawnTileInstanceAction> ptr;

    SpawnTileInstanceAction(Level* level, OpenGLTileSelector* selector, float x, float y);

    virtual void do_action();
    virtual void undo_action();
    virtual std::string get_description();

    TileInstance* get_spawned_instance() { return spawned_instance_; }

private:
    OpenGLTileSelector* selector_;
    float x_, y_;

    TileInstance* spawned_instance_;
    Tile::id_type tile_id_;
    std::string layer_name_;
};

#endif // SPAWN_TILE_INSTANCE_ACTION_H_INCLUDED
