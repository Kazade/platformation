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


#ifndef OPENGL_PICKER_H_INCLUDED
#define OPENGL_PICKER_H_INCLUDED

#include <GL/gl.h>
#include <map>
#include <iterator>
#include <tr1/memory>

template<typename T>
class OpenGLPicker {
public:
    typedef std::tr1::shared_ptr<OpenGLPicker<T> > ptr;

    OpenGLPicker() {
        reset_colour_counter();
    }

    template<typename ForwardIterator>
    T pick(float mouse_x, float mouse_y, ForwardIterator begin, ForwardIterator end) {
        glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);

        reset_colour_counter();
        items_.clear();

        for(; begin != end; ++begin) {
            PickColour c = next_colour();

            items_[c] = (*begin);

            glColor3f(float(c.r) / 255.0f,
                      float(c.g) / 255.0f,
                      float(c.b) / 255.0f);

            (*begin)->render_geometry();
        }

        unsigned char pixel[3];
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glReadPixels(mouse_x, viewport[3] - mouse_y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

        PickColour selected;
        selected.r = pixel[0];
        selected.g = pixel[1];
        selected.b = pixel[2];

        glPopAttrib();

        return items_[selected];
    }

private:
    struct PickColour {
        unsigned char r;
        unsigned char g;
        unsigned char b;

        bool operator<(const PickColour& rhs) const {
            if (r < rhs.r ||
                (r == rhs.r && g < rhs.g) ||
                (r == rhs.r && g == rhs.g && b < rhs.b)) {
                return true;
            }

            return false;
        }
    };

    PickColour current_colour_;

    void reset_colour_counter() {
        current_colour_.r = current_colour_.g = current_colour_.b = 0;
    }

    PickColour next_colour() {
        current_colour_.r++;
        if(current_colour_.r > 255) {
            current_colour_.r = 0;
            current_colour_.g++;
            if(current_colour_.g > 255) {
                current_colour_.g = 0;
                current_colour_.b++;

                assert(current_colour_.b <= 255); //Overflow
            }
        }

        return current_colour_;
    }

    std::map<PickColour, T> items_;
};

#endif // OPENGL_PICKER_H_INCLUDED
