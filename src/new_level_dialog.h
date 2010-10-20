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

    void on_browse_directory_clicked();

private:
    std::string level_name_;
    std::string tileset_path_;

    Gtk::Dialog* dialog_;
    Gtk::Entry* gtk_directory_;
    Gtk::Button* gtk_browse_directory_;

    //Gtk::TextEntry* gtk_level_name_;
};

#endif // NEW_LEVEL_DIALOG_H_INCLUDED
