#ifndef TILE_INSTANCE_H_INCLUDED
#define TILE_INSTANCE_H_INCLUDED

#include <clutter.h>

class Tileset;
class Tile;

class TileInstance {
public:
    TileInstance(Tileset* tileset, int tile_id);

    void set_position() const;
    void get_position(float& x, float& y);

    ClutterActor* get_actor() const;

private:
    Tile* tile_;
    ClutterTexture* actor_;
};

#endif // TILE_INSTANCE_H_INCLUDED
