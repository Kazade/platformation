#ifndef TILE_SELECTOR_H
#define TILE_SELECTOR_H

#include <cstdint>

#include "kglt/scene.h"

class Tileset;

const int32_t OPTION_COUNT_EACH_SIDE = 3;
const int32_t OPTION_COUNT_TOTAL = (OPTION_COUNT_EACH_SIDE * 2) + 1;

class TileSelector {
public:
	TileSelector(kglt::Scene& scene, Tileset& tileset);
	
	void init();
	void choose_tile(uint32_t i);
	void select_tile(uint32_t index);
	
	kglt::Scene& scene() { return scene_; }
	Tileset& tileset() { return tileset_; }
	
private:
	kglt::MeshID parent_mesh_;
	kglt::MeshID slots_[OPTION_COUNT_TOTAL];
	
	uint32_t selected_index_in_tileset_;
	uint32_t selected_slot_;
	
	kglt::Scene& scene_;
	Tileset& tileset_;
};

#endif
