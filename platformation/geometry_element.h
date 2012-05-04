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


#ifndef GEOMETRY_ELEMENT_H_INCLUDED
#define GEOMETRY_ELEMENT_H_INCLUDED

#include <utility>
#include <vector>
#include <kazmath/vec2.h>

#include "object.h"

enum GeometryLayer {
    GEO_BACK = -1,
    GEO_NORMAL = 0,
    GEO_FRONT = 1
};

class GeometryElement : public Object {
public:
    typedef std::tr1::shared_ptr<GeometryElement> ptr;
    typedef std::vector<kmVec2> PointArray;
    typedef std::pair<PointArray::iterator, PointArray::iterator> PointArrayIteratorPair;

    static GeometryElement create_box_from(const kmVec2& v1, const kmVec2& v2);
    static GeometryElement create_triangle_from(const kmVec2& v1, const kmVec2& v2);
    static GeometryElement create_curve_from(const kmVec2& v1, const kmVec2& v2);

    GeometryElement();
    GeometryElement(const std::vector<kmVec2> points);

    void set_layer(GeometryLayer layer);
    GeometryLayer get_layer() const { return layer_; }
    void render_geometry();

    PointArrayIteratorPair get_point_iterators() {  return std::make_pair(vertices_.begin(), vertices_.end()); }

private:
    PointArray vertices_;

    GeometryLayer layer_;
};


#endif // GEOMETRY_ELEMENT_H_INCLUDED
