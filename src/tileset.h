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

    static Tileset::ptr load_from_directory(const std::string& path);

    Tile* get_tile_by_id(Tile::id_type id) const;

    int get_tile_count() const { return tiles_.size(); }
    Tile* get_tile_at(int i) { return tiles_[i].get(); }

    IteratorPair get_iterators() {
        return std::make_pair(tiles_.begin(), tiles_.end());
    }

private:
    void add_tile(const std::string& path);

    TileArray tiles_;
};

#endif // TILESET_H_INCLUDED
