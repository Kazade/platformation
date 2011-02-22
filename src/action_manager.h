#ifndef ACTION_MANAGER_H_INCLUDED
#define ACTION_MANAGER_H_INCLUDED

#include <sigc++/sigc++.h>
#include "actions/action.h"

class ActionManager {
private:
    std::stack<Action::ptr> undo_stack_;
    std::stack<Action::ptr> redo_stack_;

    sigc::signal<void> signal_changed_;
public:
    void push_action(Action::ptr action) {
        undo_stack_.push(action);
        signal_changed_();
    }

    bool can_undo() const {
        return !undo_stack_.empty();
    }

    bool can_redo() const {
        return !redo_stack_.empty();
    }

    void undo() {
        if(!can_undo()) return;

        //Get the last action
        Action::ptr action = undo_stack_.top();
        //undo it and then remove it from the stack
        action->undo_action();
        undo_stack_.pop();

        //Add it to the redo stack
        redo_stack_.push(action);
        signal_changed_();
    }

    void redo() {
        if(!can_redo()) return;

        Action::ptr action = redo_stack_.top();
        action->do_action();
        redo_stack_.pop();

        undo_stack_.push(action);
        signal_changed_();
    }

    sigc::signal<void>& signal_changed() { return signal_changed_; }
};

#endif // ACTION_MANAGER_H_INCLUDED
