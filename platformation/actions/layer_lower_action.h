#ifndef LAYER_LOWER_ACTION_H
#define LAYER_LOWER_ACTION_H

#include "action.h"
#include "platformation/typedefs.h"

class LayerLowerAction : public Action {
public:
    LayerLowerAction(Level* level, LayerID layer);
    virtual void do_action();
    virtual void undo_action();
    virtual std::string get_description() { return "lower layer"; }
    
private:
    LayerID layer_id_;
};

#endif
