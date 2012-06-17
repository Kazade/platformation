#include <kglt/procedural/mesh.h>

#include "tile_selector.h"
#include "tileset.h"

TileSelector::TileSelector(kglt::Scene& scene, Tileset& tileset):
	scene_(scene),
	tileset_(tileset) {
	
    parent_mesh_ = scene_.new_mesh();
}

void TileSelector::init() {
	for(int32_t i = 0; i < OPTION_COUNT_TOTAL; ++i) {
		slots_[i] = scene().new_mesh();
		kglt::Mesh& mesh = scene().mesh(slots_[i]);
		kglt::procedural::mesh::rectangle(mesh, 1.0, 1.0);
        mesh.set_parent(&scene().mesh(parent_mesh_));
	}
	
	selected_index_in_tileset_ = OPTION_COUNT_EACH_SIDE + 1;
	select_tile(OPTION_COUNT_EACH_SIDE + 1);
}

void TileSelector::select_tile(uint32_t index) {
	
	/* NOTES FOR CONTINUING WITH THIS
	 * 
	 * We have 7 slots
	 * We initially populate using indexes 0-6 into the tileset
	 * If we select slot 0, then that becomes the middle tile, and
	 * we only want to display slots to the right (e.g. 4-7).
	 * Likewise, if we select the final slot, and that refers to the last
	 * tile in the tileset. Then we want to display only slots from (1-4)
	 * If we select slot 2, then we want to display slots 3-7... you get the idea.
	 */
	
	/*
	 * If we select a tile we need to look up the right index
	 * in the tileset. This will be selected_index_in_tileset_ - (midpoint - selected);
	 */
	int32_t new_index_into_tileset = selected_index_in_tileset_ - ((OPTION_COUNT_EACH_SIDE+1) - index);

	int32_t slot_id = 0;
	for(int32_t i = new_index_into_tileset - OPTION_COUNT_EACH_SIDE; i < new_index_into_tileset + OPTION_COUNT_EACH_SIDE; ++i) {
		kglt::Mesh& mesh = scene().mesh(slots_[slot_id]);
		if(i < 0) {
			mesh.set_visible(false);
		} else {
			Tile::ptr tile = tileset().tile_by_index(i);
			mesh.apply_texture(tile->texture_id());
			mesh.set_visible(true);
		}
		slot_id++;
	}
	
	selected_index_in_tileset_ = new_index_into_tileset;
	selected_slot_ = index;
}
