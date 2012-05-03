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
*  © Copyright Luke Benstead 2010
**********************************************************************************/

#ifndef TRANSPARENT_COLOUR_H_INCLUDED
#define TRANSPARENT_COLOUR_H_INCLUDED

#include <boost/cstdint.hpp>

struct TransparentColour {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    TransparentColour():
        r(255),
        g(255),
        b(255) {}

    TransparentColour(uint8_t R, uint8_t G, uint8_t B):
        r(R), g(G), b(B) {}

    TransparentColour(const TransparentColour& rhs):
        r(rhs.r), g(rhs.g), b(rhs.b) {}
};

#endif // TRANSPARENT_COLOUR_H_INCLUDED
