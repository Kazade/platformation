#include "tile_selector.h"

/** @brief on_stage_clicked
  *
  * @todo: document this function
  */
void TileSelector::on_stage_clicked(ClutterEvent* event)
{
    gfloat x = 0;
    gfloat y = 0;
    clutter_event_get_coords (event, &x, &y);
    ClutterActor* a = clutter_stage_get_actor_at_pos(CLUTTER_STAGE(stage_), CLUTTER_PICK_ALL, x, y);
    if(a && a != CLUTTER_ACTOR(stage_)) {
        Tile* t = (Tile*) g_object_get_data(G_OBJECT(a), "tile");
        active_tile_ = t->get_id();
        //std::cout << "Tile clicked: " << active_tile_ << std::endl;
    } else {
        active_tile_ = -1;
    }
}

/** @brief initialize
  *
  * @todo: document this function
  */
bool TileSelector::initialize()
{
    assert(container_);

    GtkClutterEmbed* embed = (GtkClutterEmbed*) gtk_clutter_embed_new();
    assert(embed);

    gtk_tile_canvas_ = Glib::wrap((GtkWidget*) embed);
    container_->pack_start(*Gtk::manage(gtk_tile_canvas_), Gtk::PACK_EXPAND_WIDGET);

    stage_ = CLUTTER_STAGE(gtk_clutter_embed_get_stage(embed));
    assert(stage_);

    clutter_stage_set_color((ClutterStage*)stage_, clutter_color_new(255, 255, 255, 0));

    gtk_tile_canvas_->show();
    clutter_actor_show(CLUTTER_ACTOR(stage_));

    //Connect (in a round-about way) the on_tile_stage_clicked method
    g_signal_connect(stage_, "button-press-event", G_CALLBACK(TileSelector::on_stage_clicked_cb), this);
    return true;
}

/** @brief TileSelector
  *
  * @todo: document this function
  */
TileSelector::TileSelector(Gtk::Box* container):
active_tile_(-1),
container_(container),
stage_(NULL)
{
    initialize();
}

