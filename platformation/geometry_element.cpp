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

#include "geometry_element.h"

/** @brief render_geometry
  *
  * @todo: document this function
  */
void GeometryElement::render_geometry()
{
    glBegin(GL_POLYGON);

    for(unsigned i = 0; i < vertices_.size(); ++i) {
        glVertex2f(vertices_[i].x, vertices_[i].y);
    }

    glEnd();
}

/** @brief GeometryElement
  *
  * @todo: document this function
  */
GeometryElement::GeometryElement():
layer_(GEO_NORMAL)
{

}


/** @brief GeometryElement
  *
  * @todo: document this function
  */
GeometryElement::GeometryElement(const std::vector<kmVec2> points):
vertices_(points),
layer_(GEO_NORMAL)
{

}


/** @brief create_curve_from
  *
  * @todo: document this function
  */
GeometryElement GeometryElement::create_curve_from(const kmVec2& v1, const kmVec2& v2)
{
    assert(0 && "Not implemented");
}

/** @brief create_triangle_from
  *
  * @todo: document this function
  */
GeometryElement GeometryElement::create_triangle_from(const kmVec2& v1, const kmVec2& v2)
{
    std::vector<kmVec2> vertices;

    kmVec2 p1, p2, p3;

    p1.x = v1.x;
    p1.y = v1.y;

    p2.x = v2.x;
    p2.y = v2.y;

    p3.x = v2.x;
    p3.y = v1.y;

    vertices.push_back(p1);
    vertices.push_back(p2);
    vertices.push_back(p3);

    return GeometryElement(vertices);
}

/** @brief create_box_from
  *
  * @todo: document this function
  */
GeometryElement GeometryElement::create_box_from(const kmVec2& v1, const kmVec2& v2)
{
    std::vector<kmVec2> vertices;

    kmVec2 p1, p2, p3, p4;

    p1.x = v1.x;
    p1.y = v1.y;

    p2.x = v1.x;
    p2.y = v2.y;

    p3.x = v2.x;
    p3.y = v2.y;

    p4.x = v2.x;
    p4.y = v1.y;

    ///@todo: Order CCW

    vertices.push_back(p1);
    vertices.push_back(p2);
    vertices.push_back(p3);
    vertices.push_back(p4);

    return GeometryElement(vertices);
}

/** @brief set_layer
  *
  * @todo: document this function
  */
void GeometryElement::set_layer(GeometryLayer layer)
{
    layer_ = layer;
}

