#ifndef ACTION_H_INCLUDED
#define ACTION_H_INCLUDED

#include <boost/shared_ptr.hpp>
#include "../level.h"

class Action {
public:
    typedef std::tr1::shared_ptr<Action> ptr;

    virtual ~Action() {}

    Action(Level* level):
        level_(level) {}

    virtual void do_action() = 0;
    virtual void undo_action() = 0;
    virtual std::string get_description() = 0;

protected:
    Level* level_;
};

#endif // ACTION_H_INCLUDED
