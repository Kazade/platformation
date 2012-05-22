/***********************************************************************************
*
*  This program is free software; you can redistribute it and/or modify 
*  it under the terms of the GNU Lesser General Public License as published 
*  by the Free Software Foundation; either version 3 of the License, or (at 
*  your option) any later version.
*
*  This program is distributed in the hope that it will be useful, but 
*  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
*  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public 
*  License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License 
*  along with this program; if not, see <http://www.gnu.org/copyleft/lesser.html>.
*
**********************************************************************************/


#include <GL/gl.h>
#include <cassert>

#include "kglt/procedural/mesh.h"
#include "kglt/mesh.h"

#include "layer.h"
#include "level.h"
#include "tileset.h"
#include "tile_instance.h"

/** @brief TileInstance
  *
  * @todo: document this function
  * @todo: Spawn a ClutterTexture
  */
TileInstance::TileInstance(Layer* parent, TileID tile_id):
layer_(parent),
tile_(tile_id) {    
    //Create a new mesh
    mesh_id_ = parent->level().scene().new_mesh();
    kglt::Mesh& mesh = parent->level().scene().mesh(mesh_id_);
    kglt::procedural::mesh::rectangle(mesh, 1.0, 1.0);
    
    mesh.set_user_data((void*)this); //Store the pointer to this, in the mesh
}

TileInstance::~TileInstance() {
    layer_->level().scene().delete_mesh(mesh_id_);
}

void TileInstance::mark_selected(bool value) { 
	if(value) {
		layer().level().scene().mesh(mesh_id()).set_diffuse_colour(kglt::Colour(1.0, 0.0, 0.0, 1.0));
	} else {
		layer().level().scene().mesh(mesh_id()).set_diffuse_colour(kglt::Colour(1.0, 1.0, 1.0, 1.0));
	}
}

