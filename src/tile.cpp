#include <SOIL/SOIL.h>

#include "tile.h"

/** @brief Tile
  *
  * @todo: document this function
  */
Tile::Tile(const std::string& path) {
    load_tile(path);
    id_ = next_id();
}

void Tile::load_tile(const std::string& path) {
    unsigned char* data = SOIL_load_image(path.c_str(), &width_, &height_, &channels_, SOIL_LOAD_AUTO);

    assert(data);

    data_ = std::vector<unsigned char>(data, data + (width_ * height_ * channels_));
    SOIL_free_image_data(data);
}

/** @brief get_height
  *
  * @todo: document this function
  */
int Tile::get_height() const
{
    return height_;
}

/** @brief get_width
  *
  * @todo: document this function
  */
int Tile::get_width() const
{
    return width_;
}

/** @brief get_channels
  *
  * @todo: document this function
  */
int Tile::get_channels() const
{
    return channels_;
}

