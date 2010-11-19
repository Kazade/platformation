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


#ifndef LAYER_H_INCLUDED
#define LAYER_H_INCLUDED

#include <list>
#include <utility>
#include <string>

#include <boost/shared_ptr.hpp>
#include "tile_instance.h"

class Layer {
public:
    typedef boost::shared_ptr<Layer> ptr;

    typedef std::list<TileInstance::ptr> TileList;
    typedef std::pair<TileList::iterator, TileList::iterator> TileListIteratorPair;

    Layer(Tileset* tileset);
    TileInstance* spawn_tile_instance(int tile_id, bool select=true);

    int get_tile_instance_count() const;
    TileInstance* get_tile_instance_at(int i) const;
    void delete_tile_instance(TileInstance* instance);

    TileListIteratorPair get_iterators() {
        return std::make_pair(tile_instances_.begin(), tile_instances_.end());
    }

    void set_name(const std::string& name);
    std::string get_name() const;

private:
    TileList tile_instances_;
    Tileset* tileset_;
    std::string name_;
};

#endif // LAYER_H_INCLUDED
