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

    LayerManager(Gtk::TreeView* view);

    void on_layer_created(Layer* layer);
    void on_layer_destroyed(Layer* layer);
    void set_level(Level* level);

private:
    Gtk::TreeView* view_;
    Level* level_;
};

#endif // LAYER_MANAGER_H_INCLUDED
