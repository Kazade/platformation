#ifndef OPENGL_PICKER_H_INCLUDED
#define OPENGL_PICKER_H_INCLUDED

template<typename T>
class OpenGLPicker {
public:
    template<typename input_iterator>
    T* pick(float mouse_x, float mouse_y, input_iterator begin, input_iterator end) {
        glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);

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

        unsigned char pixel[3]
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glReadPixels(mouse_x, viewport[3] - mouse_y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

        PickColour selected;
        selected.r = pixel[0];
        selected.g = pixel[1];
        selected.b = pixel[2];

        glPopAttrib();

        return map_[selected];
    }

private:
    struct PickColour {
        unsigned char r;
        unsigned char g;
        unsigned char b;
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

    std::map<PickColour, T* item> items_;
};

#endif // OPENGL_PICKER_H_INCLUDED
