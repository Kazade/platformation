#ifndef TILE_INSTANCE_H_INCLUDED
#define TILE_INSTANCE_H_INCLUDED

#include <boost/shared_ptr.hpp>

#include "object.h"

class Tileset;
class Tile;

class TileInstance : public Object {
public:
    typedef boost::shared_ptr<TileInstance> ptr;

    TileInstance(Tileset* tileset, int tile_id);

    void render_geometry();

    Tile* get_tile() { return tile_; }
private:
    Tile* tile_;

};

#endif // TILE_INSTANCE_H_INCLUDED
