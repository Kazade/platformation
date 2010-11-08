#include <cassert>
#include <boost/filesystem.hpp>

#include "new_level_dialog.h"

#define UI_FILE "ui/new_level.glade"

namespace bfs = boost::filesystem;

NewLevelDialog::NewLevelDialog():
dialog_(NULL) {
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
    builder->add_from_file(UI_FILE);

    builder->get_widget("new_level_dialog", dialog_);
    //builder->get_widget("level_name_entry", gtk_level_name_);
    builder->get_widget("directory_entry", gtk_directory_);
    builder->get_widget("browse_directory", gtk_browse_directory_);
    builder->get_widget("level_name", gtk_level_name_);

    assert(dialog_);
    assert(gtk_browse_directory_);
    assert(gtk_level_name_);

    gtk_browse_directory_->signal_clicked().connect(sigc::mem_fun(this, &NewLevelDialog::on_browse_directory_clicked));

    builder->get_widget("ok_button", gtk_ok_button_);
    assert(gtk_ok_button_);
    gtk_ok_button_->set_sensitive(false);
    //dialog_->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    //dialog_->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

    gtk_directory_->signal_changed().connect(sigc::mem_fun(this, &NewLevelDialog::on_entry_changed));
    gtk_level_name_->signal_changed().connect(sigc::mem_fun(this, &NewLevelDialog::on_entry_changed));
}

/**
    @todo Check that the directory and level name are valid
*/
void NewLevelDialog::on_entry_changed() {
    if(gtk_directory_->get_text_length() && gtk_level_name_->get_text_length()) {
        gtk_ok_button_->set_sensitive(true);
    } else {
        gtk_ok_button_->set_sensitive(false);
    }
}

int NewLevelDialog::run_dialog(Gtk::Window* parent) {
    gtk_level_name_->set_text(""); //Clear the level name on load

    dialog_->set_transient_for(*parent);

    int result = dialog_->run();
    dialog_->hide();

    level_name_ = gtk_level_name_->get_text().raw();
    tileset_path_ = gtk_directory_->get_text().raw();
    return result;
}

void NewLevelDialog::on_browse_directory_clicked() {
    Gtk::FileChooserDialog dialog("Please choose a folder", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
    dialog.set_transient_for(*dialog_);

    //Attempt to set the default directory to ~/Pictures/Tilesets if it exists
    bfs::path home(getenv("USERPROFILE") ? getenv("USERPROFILE") : getenv("HOME"));
    bfs::path tileset_dir = home / "Pictures" / "Tilesets";
    if(bfs::exists(tileset_dir)) {
        dialog.set_current_folder(tileset_dir.string());
    }

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

/** @brief get_level_name
  *
  * @todo: document this function
  */
const std::string& NewLevelDialog::get_level_name() const
{
    return level_name_;
}

/** @brief get_tileset_path
  *
  * @todo: document this function
  */
const std::string& NewLevelDialog::get_tileset_path() const
{
    return tileset_path_;
}

