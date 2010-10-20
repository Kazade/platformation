#ifndef TILESET_H_INCLUDED
#define TILESET_H_INCLUDED

#include <boost/shared_ptr.hpp>
#include <map>
#include <string>
#include <clutter/clutter.h>

#include "tile.h"

class Tileset {
public:
    typedef boost::shared_ptr<Tileset> ptr;

    static Tileset::ptr load_from_directory(const std::string& path);
    void update_stage(ClutterStage* stage);

private:
    std::map<std::string, Tile::ptr> tiles_;
};

#endif // TILESET_H_INCLUDED
