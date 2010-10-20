#include <cassert>

#include "new_level_dialog.h"

#define UI_FILE "ui/new_level.glade"

NewLevelDialog::NewLevelDialog():
dialog_(NULL) {
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
    builder->add_from_file(UI_FILE);

    builder->get_widget("new_level_dialog", dialog_);
    //builder->get_widget("level_name_entry", gtk_level_name_);
    builder->get_widget("directory_entry", gtk_directory_);
    builder->get_widget("browse_directory", gtk_browse_directory_);
    assert(dialog_);
    assert(gtk_browse_directory_);

    gtk_browse_directory_->signal_clicked().connect(sigc::mem_fun(this, &NewLevelDialog::on_browse_directory_clicked));
}

int NewLevelDialog::run_dialog(Gtk::Window* parent) {
    dialog_->set_transient_for(*parent);

    int result = dialog_->run();

    return result;
}

void NewLevelDialog::on_browse_directory_clicked() {
    Gtk::FileChooserDialog dialog("Please choose a folder", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
    dialog.set_transient_for(*dialog_);

    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button("Select", Gtk::RESPONSE_OK);

    int result = dialog.run();
    dialog.hide();

    switch(result) {
        case Gtk::RESPONSE_OK:
            gtk_directory_->set_text(dialog.get_filename());
            break;
        default:
            break;
    }
}
