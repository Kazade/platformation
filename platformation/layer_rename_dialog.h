#ifndef LAYER_RENAME_DIALOG_H_INCLUDED
#define LAYER_RENAME_DIALOG_H_INCLUDED

#include <tr1/memory>
#include <cassert>
#include <string>
#include <gtkmm.h>

class LayerRenameDialog {
public:
    typedef std::tr1::shared_ptr<LayerRenameDialog> ptr;

    LayerRenameDialog();

    int run_dialog();
    const std::string& get_layer_name() const;

    void on_layer_name_changed();
private:
    std::string layer_name_;
    Gtk::Dialog* dialog_;
    Gtk::Entry* gtk_layer_name_;
    Gtk::Button* gtk_ok_button_;
};

#endif // LAYER_RENAME_DIALOG_H_INCLUDED
