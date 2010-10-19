#ifndef NEW_LEVEL_DIALOG_H_INCLUDED
#define NEW_LEVEL_DIALOG_H_INCLUDED

#include <gtkmm.h>

class NewLevelDialog {
public:
    NewLevelDialog();

    void run_dialog();

    const std::string& get_level_name() const;
    const std::string& get_tileset_path() const;

private:
    std::string level_name_;
    std::string tileset_path_;

    Gtk::Dialog* dialog_;
};

#endif // NEW_LEVEL_DIALOG_H_INCLUDED
