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

TileInstance::TileInstance(Layer* parent, TileID tile_id):
layer_(parent),
tile_(tile_id) {    
    //Create a new mesh
    mesh_id_ = parent->level().scene().new_mesh();
    kglt::Mesh& mesh = parent->level().scene().mesh(mesh_id_);
    kglt::procedural::mesh::rectangle(mesh, 1.0, 1.0);    
    mesh.set_user_data((void*)this); //Store the pointer to this, in the mesh
    mesh.set_diffuse_colour(kglt::Colour(1.0, 1.0, 1.0, 0.0));
    
	//Create the outline mesh, make it non-selectable and a parent of the mesh
	outline_mesh_id_ = layer().level().scene().new_mesh();
	kglt::procedural::mesh::rectangle_outline(outline_mesh(), 1.0, 1.0);
	outline_mesh().set_parent(&mesh);
	outline_mesh().move_to(0.0, 0.0, 0.1); //Move the outline slightly closer to the camera than the mesh
}

TileInstance::~TileInstance() {
    layer_->level().scene().delete_mesh(mesh_id_);
}

void TileInstance::mark_selected(bool value) { 
	if(value) {
		//outline_mesh().set_selectable(false);    				
		outline_mesh().set_diffuse_colour(kglt::Colour(1.0, 0.0, 0.0, 1.0));
		outline_mesh().move_to(0.0, 0.0, 0.2);
	} else {
		outline_mesh().set_diffuse_colour(kglt::Colour(1.0, 1.0, 1.0, 1.0));
		outline_mesh().move_to(0.0, 0.0, 0.1);
	}
}

kglt::Mesh& TileInstance::outline_mesh() { 
	return layer().level().scene().mesh(outline_mesh_id_);
}
