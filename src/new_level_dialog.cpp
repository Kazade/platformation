#include <cassert>

#include "new_level_dialog.h"

#define UI_FILE "ui/new_level_dialog.glade"

NewLevelDialog::NewLevelDialog():
dialog_(NULL) {
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
    builder->add_from_file(UI_FILE);

    builder->get_widget("new_level_dialog", dialog_);
    //builder->get_widget("level_name_entry", gtk_level_name_);
    assert(dialog_);
}

int NewLevelDialog::run_dialog() {
    int result = dialog_->run();

    return result;
}
