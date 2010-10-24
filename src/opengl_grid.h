#ifndef OPENGL_GRID_H_INCLUDED
#define OPENGL_GRID_H_INCLUDED

#include <boost/shared_ptr.hpp>

struct GridColour {
    float r, g, b;
};

class OpenGLGrid {
public:
    typedef boost::shared_ptr<OpenGLGrid> ptr;

    OpenGLGrid(float small_step, int big_step_frequency, const GridColour& colour);
    void render();

    void set_small_step(float x);
    void set_big_step_frequency(int i);
    void set_colour(const GridColour& c);

private:
    float small_step_;
    int big_step_frequency_;
    GridColour colour_;
};

#endif // OPENGL_GRID_H_INCLUDED
