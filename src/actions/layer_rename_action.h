#ifndef LAYER_RENAME_ACTION_H_INCLUDED
#define LAYER_RENAME_ACTION_H_INCLUDED

#include <string>

#include "action.h"

class LayerRenameAction : public Action {
public:
    LayerRenameAction(Level* level, const std::string& old_name, const std::string& new_name);

    virtual void do_action();
    virtual void undo_action();
    virtual std::string get_description();

private:
    std::string old_name_;
    std::string new_name_;

};

#endif // LAYER_RENAME_ACTION_H_INCLUDED
