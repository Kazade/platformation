#include "layer.h"

using namespace std;

Layer::Layer(Tileset* tileset):
tileset_(tileset),
name_("Untitled") {

}

TileInstance* Layer::spawn_tile_instance(int tile_id, bool select) {
    TileInstance::ptr new_tile_instance(new TileInstance(tileset_, tile_id));
    tile_instances_.push_back(new_tile_instance);
    return new_tile_instance.get();
}

int Layer::get_tile_instance_count() const {
    assert(0);
    return 0;
}

TileInstance* Layer::get_tile_instance_at(int i) const {
    assert(0);
    return NULL;
}

void Layer::delete_tile_instance(TileInstance* instance) {
    TileListIteratorPair iters = get_iterators();

    for(; iters.first != iters.second; ++iters.first) {
        TileInstance* rhs = (*iters.first).get();
        if(rhs == instance) {
            break;
        }
    }

    if(iters.first == iters.second) {
        return;
    }

    tile_instances_.erase(iters.first);
}

void Layer::set_name(const std::string& name) {
    name_ = name;
}

string Layer::get_name() const {
    return name_;
}
