#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED

#include <boost/shared_ptr.hpp>
#include <clutter/clutter.h>
#include <string>

class Tile {
public:
    typedef boost::shared_ptr<Tile> ptr;
    typedef int id_type;

    Tile(const std::string& path);

    ClutterTexture* get_texture() const;
    ClutterTexture* get_solidity_front() const;
    ClutterTexture* get_solidity_back() const;

    id_type get_id() const { return id_; }
private:
    std::string path_;
    ClutterTexture* texture_;
    Tile::id_type id_;

    static Tile::id_type next_id() {
        static Tile::id_type id = 0;
        return id++;
    }
};


#endif // TILE_H_INCLUDED
