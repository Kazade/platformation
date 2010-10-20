#include <boost/filesystem.hpp>
#include <iostream>

#include "tileset.h"

namespace bfs = boost::filesystem;

Tileset::ptr Tileset::load_from_directory(const std::string& path) {
    Tileset::ptr tileset(new Tileset);

    bfs::path p(path);

    for(bfs::directory_iterator file(path), filesEnd; file != filesEnd; ++file) {
        if(!bfs::is_directory(*file) && bfs::extension(*file) == ".png") {
            tileset->add_tile(file->string());
        }
    }

    return tileset;
}

void Tileset::update_stage(ClutterStage* stage) {

}

/** @brief add_tile
  *
  * @todo: document this function
  */
void Tileset::add_tile(const std::string& path) {
    Tile::ptr new_tile(new Tile(path));
    tiles_.push_back(new_tile);
}

