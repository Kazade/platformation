#ifndef LAYER_RAISE_ACTION_H
#define LAYER_RAISE_ACTION_H

#include "action.h"
#include "platformation/typedefs.h"

class LayerRaiseAction : public Action {
public:
    LayerRaiseAction(Level* level, LayerID layer);
    virtual void do_action();
    virtual void undo_action();
    virtual std::string get_description() { return "raise layer"; }
    
private:
    LayerID layer_id_;
};

#endif
