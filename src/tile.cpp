
#include "tile.h"

/** @brief Tile
  *
  * @todo: document this function
  */
Tile::Tile(const std::string& path) {
    GError* error = NULL;
    texture_ = CLUTTER_TEXTURE(clutter_texture_new_from_file(path.c_str(), &error));

    assert(error == NULL); //TODO: Handle this

    id_ = next_id();

    g_object_set_data(G_OBJECT(texture_), "tile", this);
}

/** @brief get_texture
  *
  * @todo: document this function
  */
ClutterTexture* Tile::get_texture() const {
    return texture_;
}

/** @brief get_solidity_front
  *
  * @todo: document this function
  */
ClutterTexture* Tile::get_solidity_front() const {

}

/** @brief get_solidity_back
  *
  * @todo: document this function
  */
ClutterTexture* Tile::get_solidity_back() const {

}

