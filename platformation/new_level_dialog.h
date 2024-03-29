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


#ifndef NEW_LEVEL_DIALOG_H_INCLUDED
#define NEW_LEVEL_DIALOG_H_INCLUDED

#include <gtkmm.h>

#include <tr1/memory>

class NewLevelDialog {
public:
    typedef std::tr1::shared_ptr<NewLevelDialog> ptr;

    NewLevelDialog();

    int run_dialog(Gtk::Window* parent=NULL);

    const std::string& get_level_name() const;
    const std::string& get_tileset_path() const;
    const std::pair<int, int> get_level_size() const;
    const void get_transparent_colour(uint8_t& r, uint8_t& g, uint8_t& b);

    void on_browse_directory_clicked();
    void on_entry_changed();

private:
    struct ModelColumns : public Gtk::TreeModel::ColumnRecord {
        ModelColumns() { add(column_id_); add(column_name_); }
        Gtk::TreeModelColumn<int> column_id_;
        Gtk::TreeModelColumn<Glib::ustring> column_name_;
    };

    ModelColumns size_columns_;

    std::string level_name_;
    std::string tileset_path_;
    std::pair<int, int> chosen_level_size_;

    Gtk::Dialog* dialog_;
    Gtk::Entry* gtk_directory_;
    Gtk::Entry* gtk_level_name_;
    Gtk::Button* gtk_browse_directory_;
    Gtk::Button* gtk_ok_button_;
    Gtk::ComboBox* gtk_level_size_;
    Gtk::ColorButton* gtk_transparent_colour_button_;

    Glib::RefPtr<Gtk::ListStore> level_size_model_;
    //Gtk::TextEntry* gtk_level_name_;
};

#endif // NEW_LEVEL_DIALOG_H_INCLUDED
