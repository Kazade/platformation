#ifndef HEADER_EC5FFDC8A3CC3ACD
#define HEADER_EC5FFDC8A3CC3ACD

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

#include <sigc++/sigc++.h>
#include <tr1/memory>

#include "tileset.h"
#include "tile_instance.h"

class Level;

class Layer {
public:
    typedef std::tr1::shared_ptr<Layer> ptr;

    typedef std::list<TileInstance::ptr> TileList;
    typedef std::pair<TileList::iterator, TileList::iterator> TileListIteratorPair;

    Layer(Level* parent, const std::string& name);
    void resize(uint32_t horizontal_tile_count, uint32_t vertical_tile_count);
    void set_tile_at(uint32_t index, TileID tile_id);

    void set_name(const std::string& name);
    std::string name() const;

    sigc::signal<void>& signal_changed() { return signal_changed_; }

    Level& level() { return *level_; }
    
    static LayerID generate_layer_id() {
        static LayerID counter = 0;
        return ++counter;
    }
    
    LayerID id() const { return id_; }
private:
    LayerID id_;
    Level* level_;
    std::string name_;
    
    sigc::signal<void> signal_changed_;
    
    std::vector<TileInstance::ptr> tile_instances_;
};

#endif // LAYER_H_INCLUDED

#endif // header guard
