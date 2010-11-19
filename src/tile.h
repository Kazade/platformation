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


#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED

#include <utility>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <string>

#include "object.h"
#include "geometry_element.h"

class Tile : public Object {
public:
    typedef boost::shared_ptr<Tile> ptr;
    typedef int id_type;
    typedef std::vector<GeometryElement::ptr> GeometryArray;
    typedef std::pair<GeometryArray::iterator, GeometryArray::iterator > GeometryIteratorPair;

    Tile(const std::string& path);

    id_type get_id() const { return id_; }

    //Probably should be in a separate place than here
    int get_width() const;
    int get_height() const;
    unsigned char* get_data() { return &data_[0]; }
    int get_channels() const;

    void render_geometry();

    std::pair<float, float> get_rendered_dimensions() const;

    void add_geometry_element(GeometryElement::ptr element);
    int get_geometry_element_count() const { return (int) geometry_.size(); }
    GeometryElement* get_geometry_element_at(int i) { return geometry_[i].get(); }
    void delete_geometry_element(GeometryElement* element);

    GeometryIteratorPair get_geometry_iterators() {
        return std::make_pair(geometry_.begin(), geometry_.end());
    }

    bool save();
    bool load();

    std::string get_name() const;
private:
    std::string path_;

    int width_;
    int height_;
    int channels_;
    std::vector<unsigned char> data_;

    void load_tile(const std::string& path);

    Tile::id_type id_;

    static Tile::id_type next_id() {
        static Tile::id_type id = 0;
        return id++;
    }

    GeometryArray geometry_;
};


#endif // TILE_H_INCLUDED
