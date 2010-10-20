#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED

#include <boost/shared_ptr.hpp>
#include <clutter/clutter.h>
#include <string>

class Tile {
public:
    typedef boost::shared_ptr<Tile> ptr;

    Tile(const std::string& path);

    ClutterTexture* get_texture() const;
    ClutterTexture* get_solidity_front() const;
    ClutterTexture* get_solidity_back() const;

private:
    std::string path_;
    ClutterTexture* texture_;
};


#endif // TILE_H_INCLUDED
