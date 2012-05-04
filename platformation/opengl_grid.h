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


#ifndef OPENGL_GRID_H_INCLUDED
#define OPENGL_GRID_H_INCLUDED

#include <tr1/memory>
#include <cassert>

struct GridColour {
    float r, g, b;
};

class OpenGLGrid {
public:
    typedef std::tr1::shared_ptr<OpenGLGrid> ptr;

    OpenGLGrid(float small_step, int big_step_frequency, const GridColour& colour);
    void render();

    void set_small_step(float x);
    void set_big_step_frequency(int i);
    void set_colour(const GridColour& c);

    void snap_to(double& x, double& y);
    void snap_to(float& x, float& y);

    void set_min_x(float x);
    void set_max_x(float x);
    void set_min_y(float y);
    void set_max_y(float y);
private:
    float small_step_;
    int big_step_frequency_;
    GridColour colour_;

    float min_x_;
    float max_x_;
    float min_y_;
    float max_y_;
};

#endif // OPENGL_GRID_H_INCLUDED
