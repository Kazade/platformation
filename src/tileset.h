#ifndef TILESET_H_INCLUDED
#define TILESET_H_INCLUDED

#include <string>

class Tileset {
public:
    typedef boost::shared_ptr<Tileset> ptr;

    static Tileset::ptr load_from_directory(const std::string& path);

private:
    std::map<std::string, Tile::ptr> tiles_;
};

#endif // TILESET_H_INCLUDED
