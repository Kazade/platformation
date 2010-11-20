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


#ifndef TILESET_H_INCLUDED
#define TILESET_H_INCLUDED

#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>
#include <iterator>
#include <utility>

#include "tile.h"

class Tileset {
public:
    typedef boost::shared_ptr<Tileset> ptr;
    typedef std::vector<Tile::ptr> TileArray;
    typedef std::pair<TileArray::iterator, TileArray::iterator > IteratorPair;

    static Tileset::ptr load_from_directory(const std::string& path, const TransparentColour& c);

    Tile* get_tile_by_id(Tile::id_type id) const;

    int get_tile_count() const { return tiles_.size(); }
    Tile* get_tile_at(int i) { return tiles_[i].get(); }

    IteratorPair get_iterators() {
        return std::make_pair(tiles_.begin(), tiles_.end());
    }

    void sort_tiles();
    void set_transparent_colour(const TransparentColour& c);
private:
    void add_tile(const std::string& path);

    TileArray tiles_;
    TransparentColour transparent_colour_;
};

#endif // TILESET_H_INCLUDED
