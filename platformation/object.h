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


#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <boost/shared_ptr.hpp>
#include <kazmath/vec2.h>

class Object {
public:
    typedef boost::shared_ptr<Object> ptr;

    virtual ~Object() {}

    kmVec2 get_position() const { return position_; }
    void set_position(float x, float y) { position_.x = x; position_.y = y; }

    virtual void render_geometry() = 0;
private:
    kmVec2 position_;
};

#endif // OBJECT_H_INCLUDED
