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
