#ifndef LAYER_MANAGER_H_INCLUDED
#define LAYER_MANAGER_H_INCLUDED

#include <boost/signals.hpp>

#include <gtkmm.h>

class Level;
class Layer;

/*
    Keeps the layer list view updated and in
    sync with the level's layers
*/
class LayerManager {
public:
    typedef boost::shared_ptr<LayerManager> ptr;

    LayerManager(Gtk::TreeView* view, Gtk::Button* add_layer_button, Gtk::Button* delete_layer_button);

    void on_layer_created(Layer* layer);
    void on_layer_destroyed(Layer* layer);
    void set_level(Level* level);

    void on_layer_manager_row_activate(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* /* column */);

    void on_add_layer_clicked();
    void on_delete_layer_clicked();

    Layer* get_active_layer();
private:
    Gtk::TreeView* view_;
    Gtk::Button* add_button_;
    Gtk::Button* delete_button_;

    Glib::RefPtr<Gtk::TreeStore> tree_model_;

    struct ModelColumns : public Gtk::TreeModel::ColumnRecord {
        ModelColumns() { add(column_id_); add(column_name_); }
        Gtk::TreeModelColumn<int> column_id_;
        Gtk::TreeModelColumn<Glib::ustring> column_name_;
    };
    ModelColumns columns_;

    Level* level_;

    void update_list_view();
};

#endif // LAYER_MANAGER_H_INCLUDED
