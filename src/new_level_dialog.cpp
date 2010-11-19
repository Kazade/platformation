/***********************************************************************************
*
*  This program is free software; you can redistribute it and/or modify 
*  it under the terms of the GNU Lesser General Public License as published 
*  by the Free Software Foundation; either version 3 of the License, or (at 
*  your option) any later version.
*
*  This program is distributed in the hope that it will be useful, but 
*  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
*  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public 
*  License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License 
*  along with this program; if not, see <http://www.gnu.org/copyleft/lesser.html>.
*
**********************************************************************************/


#include <cassert>
#include <boost/filesystem.hpp>

#include "new_level_dialog.h"

#define UI_FILE "ui/new_level.glade"

namespace bfs = boost::filesystem;

NewLevelDialog::NewLevelDialog():
dialog_(NULL),
gtk_directory_(NULL),
gtk_browse_directory_(NULL),
gtk_level_name_(NULL),
gtk_level_size_(NULL) {
    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
    builder->add_from_file(UI_FILE);

    builder->get_widget("new_level_dialog", dialog_);
    //builder->get_widget("level_name_entry", gtk_level_name_);
    builder->get_widget("directory_entry", gtk_directory_);
    builder->get_widget("browse_directory", gtk_browse_directory_);
    builder->get_widget("level_name", gtk_level_name_);
    builder->get_widget("level_size_box", gtk_level_size_);

    assert(dialog_);
    assert(gtk_browse_directory_);
    assert(gtk_level_name_);
    assert(gtk_level_size_);

    gtk_browse_directory_->signal_clicked().connect(sigc::mem_fun(this, &NewLevelDialog::on_browse_directory_clicked));

    builder->get_widget("ok_button", gtk_ok_button_);
    assert(gtk_ok_button_);
    gtk_ok_button_->set_sensitive(false);
    //dialog_->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    //dialog_->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);


    level_size_model_ = Gtk::ListStore::create(size_columns_);
    gtk_level_size_->set_model(level_size_model_);
    Gtk::TreeModel::Row row = *(level_size_model_->append());
    row[size_columns_.column_id_] = 1;
    row[size_columns_.column_name_] = "Small (10240px x 1280px)";

    //gtk_level_size_->pack_start(size_columns_.column_id_);
    gtk_level_size_->pack_start(size_columns_.column_name_);
    gtk_level_size_->set_active(0);

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

    chosen_level_size_ = std::make_pair(10240, 1280);

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

const std::pair<int, int> NewLevelDialog::get_level_size() const
{
    return chosen_level_size_;
}

