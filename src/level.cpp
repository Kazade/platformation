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
TileInstance* Level::get_tile_instance_at(int i) const
{
    assert(0);
    return NULL;
}

/** @brief get_tile_instance_count
  *
  * @todo: document this function
  */
int Level::get_tile_instance_count() const
{
    assert(0);
    return 0;
}

/** @brief spawn_tile_instance
  *
  * @todo: document this function
  */
TileInstance* Level::spawn_tile_instance(int tile_id, bool select)
{
    TileInstance::ptr new_tile_instance(new TileInstance(tileset_, tile_id));
    tile_instances_.push_back(new_tile_instance);
    return new_tile_instance.get();
}

/** @brief Level
  *
  * @todo: document this function
  */
Level::Level(Tileset* tileset):
tileset_(tileset)
{

}

/** @brief delete_tile_instance
  *
  * @todo: document this function
  */
void Level::delete_tile_instance(TileInstance* instance)
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
}

