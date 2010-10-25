#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <kazmath/vec2.h>

class Object {
public:
    virtual ~Object() {}

    kmVec2 get_position() const { return position_; }
    void set_position(float x, float y) { position_.x = x; position_.y = y; }

private:
    kmVec2 position_;
};

#endif // OBJECT_H_INCLUDED
