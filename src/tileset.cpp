/***********************************************************************************
*
*  This program is free software; you can redistribute it and/or modify 
*  it under the terms of the GNU Lesser General Public License as published 
*  by the Free Software Foundation; either version 3 of the License, or (at 
*  your option) any later version.
*
*  This program is distributed in the hope that it will be useful, but 
*  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
*  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public 
*  License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License 
*  along with this program; if not, see <http://www.gnu.org/copyleft/lesser.html>.
*
**********************************************************************************/


#include <boost/filesystem.hpp>
#include <iostream>

#include "natural_sort.h"
#include "tileset.h"

namespace bfs = boost::filesystem;

bool TileSortPredicate(const Tile::ptr d1, const Tile::ptr d2)
{
    return natural_sort_predicate(d1->get_name(), d2->get_name());
}


void Tileset::sort_tiles() {
    std::sort(tiles_.begin(), tiles_.end(), TileSortPredicate);


    for(unsigned int i = 0; i < tiles_.size(); ++i) {
        std::cout << tiles_[i]->get_name() << std::endl;
    }

}

Tileset::ptr Tileset::load_from_directory(const std::string& path) {
    Tileset::ptr tileset(new Tileset);

    bfs::path p(path);

    for(bfs::directory_iterator file(path), filesEnd; file != filesEnd; ++file) {
        if(!bfs::is_directory(*file) && bfs::extension(*file) == ".png") {
            tileset->add_tile(file->string());
        }
    }

    tileset->sort_tiles();

    return tileset;
}

/** @brief add_tile
  *
  * @todo: document this function
  */
void Tileset::add_tile(const std::string& path) {
    Tile::ptr new_tile(new Tile(path));

    new_tile->load();

    tiles_.push_back(new_tile);
}
/** @brief get_tile_by_id
  *
  * @todo: document this function
  */
Tile* Tileset::get_tile_by_id(Tile::id_type id) const
{
    for(TileArray::const_iterator it = tiles_.begin(); it != tiles_.end(); ++it) {
        if((*it)->get_id() == id)
            return (*it).get();
    }

    return NULL;
}



