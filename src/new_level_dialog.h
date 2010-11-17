#ifndef NEW_LEVEL_DIALOG_H_INCLUDED
#define NEW_LEVEL_DIALOG_H_INCLUDED

#include <gtkmm.h>

#include <boost/shared_ptr.hpp>

class NewLevelDialog {
public:
    typedef boost::shared_ptr<NewLevelDialog> ptr;

    NewLevelDialog();

    int run_dialog(Gtk::Window* parent=NULL);

    const std::string& get_level_name() const;
    const std::string& get_tileset_path() const;
    const std::pair<int, int> get_level_size() const;

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
    Glib::RefPtr<Gtk::ListStore> level_size_model_;
    //Gtk::TextEntry* gtk_level_name_;
};

#endif // NEW_LEVEL_DIALOG_H_INCLUDED
