#include "layer_rename_dialog.h"

#define UI_FILE "ui/main_window.glade"

LayerRenameDialog::LayerRenameDialog():
    dialog_(NULL) {

    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
    builder->add_from_file(UI_FILE);

    builder->get_widget("layer_rename_dialog", dialog_);
    builder->get_widget("layer_name", gtk_layer_name_);
    assert(dialog_);
    assert(gtk_layer_name_);

    builder->get_widget("ok_button", gtk_ok_button_);
    assert(gtk_ok_button_);
    gtk_ok_button_->set_sensitive(false);

    gtk_layer_name_->signal_changed().connect(sigc::mem_fun(this, &LayerRenameDialog::on_layer_name_changed));
}

void LayerRenameDialog::on_layer_name_changed() {
    if(gtk_layer_name_->get_text_length()) {
        gtk_ok_button_->set_sensitive(true);
    } else {
        gtk_ok_button_->set_sensitive(false);
    }
}

int LayerRenameDialog::run_dialog() {
    gtk_layer_name_->set_text(""); //Clear the level name on load

    int result = dialog_->run();
    dialog_->hide();

    layer_name_ = gtk_layer_name_->get_text().raw();
    return result;
}

const std::string& LayerRenameDialog::get_layer_name() const {
    return layer_name_;
}
