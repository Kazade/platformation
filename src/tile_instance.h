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

#include <boost/shared_ptr.hpp>

#include "object.h"

class Tileset;
class Tile;

class TileInstance : public Object {
public:
    typedef boost::shared_ptr<TileInstance> ptr;

    TileInstance(Tileset* tileset, int tile_id);

    void render_geometry();

    Tile* get_tile() { return tile_; }
private:
    Tile* tile_;

};

#endif // TILE_INSTANCE_H_INCLUDED
