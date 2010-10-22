#include "level.h"

/** @brief load
  *
  * @todo: document this function
  */
bool Level::load(const std::string& filename)
{

}

/** @brief save
  *
  * @todo: document this function
  */
bool Level::save(const std::string& filename) const
{

}

/** @brief get_tile_instance_at
  *
  * @todo: document this function
  */
TileInstance* Level::get_tile_instance_at(int i) const
{

}

/** @brief get_tile_instance_count
  *
  * @todo: document this function
  */
int Level::get_tile_instance_count() const
{

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

