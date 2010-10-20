#include <boost/filesystem.hpp>
#include <iostream>

#include "tileset.h"

namespace bfs = boost::filesystem;

Tileset::ptr Tileset::load_from_directory(const std::string& path) {
    Tileset::ptr tileset(new Tileset);

    bfs::path p(path);
    bfs::path::iterator file = p.begin();
    while(file != p.end()) {
        std::cout << *file << std::endl;
        ++file;
    }

    return Tileset::ptr();
}

void Tileset::update_stage(ClutterStage* stage) {

}
