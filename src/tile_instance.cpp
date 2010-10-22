#include "tileset.h"
#include "tile_instance.h"

/** @brief TileInstance
  *
  * @todo: document this function
  * @todo: Spawn a ClutterTexture
  */
TileInstance::TileInstance(Tileset* tileset, int tile_id):
tile_(NULL)
{
    tile_ = tileset->get_tile_by_id(tile_id);
}

/** @brief set_position
  *
  * @todo: document this function
  */
void TileInstance::set_position() const
{

}

/** @brief get_position
  *
  * @todo: document this function
  */
void TileInstance::get_position(float& x, float& y)
{

}

/** @brief get_actor
  *
  * @todo: document this function
  */
ClutterActor* TileInstance::get_actor() const
{

}

