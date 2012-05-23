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


#ifndef TILE_INSTANCE_H_INCLUDED
#define TILE_INSTANCE_H_INCLUDED

#include <tr1/memory>
#include "typedefs.h"
#include "kglt/types.h"

class Tileset;
class Layer;

class TileInstance {
public:
    typedef std::tr1::shared_ptr<TileInstance> ptr;

    TileInstance(Layer* parent, TileID tile_id=0);
    ~TileInstance();
    
    TileID tile_id() { return tile_; }
    Layer& layer() { return *layer_; }
    kglt::MeshID mesh_id() const { return mesh_id_; }
    
    void mark_selected(bool value=true);
    
private:
	kglt::Mesh& outline_mesh();

    Layer* layer_;
    TileID tile_;
    kglt::MeshID mesh_id_;
    kglt::MeshID outline_mesh_id_;
};

#endif // TILE_INSTANCE_H_INCLUDED
