#include "tileset.h"

namespace bfs = boost::filesystem;

Tileset::ptr Tileset::load_from_directory(const std::string& path) {
    Tileset::ptr tileset(new Tileset);

    bfs::path p(path);
    bfs::path::iterator file = p.begin();
    while(file != p.end()) {
        std::cout << (*file).str() << std::endl;
        ++file;
    }

    return Tileset::ptr();
}
