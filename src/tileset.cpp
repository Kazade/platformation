#include <boost/filesystem.hpp>
#include <iostream>

#include "tileset.h"

namespace bfs = boost::filesystem;

Tileset::ptr Tileset::load_from_directory(const std::string& path) {
    Tileset::ptr tileset(new Tileset);

    bfs::path p(path);

    for(bfs::directory_iterator file(path), filesEnd; file != filesEnd; ++file) {
        if(!bfs::is_directory(*file) && bfs::extension(*file) == ".png") {
            tileset->add_tile(file->string());
        }
    }

    return tileset;
}

void Tileset::update_stage(ClutterStage* stage) {
    GList *children = clutter_container_get_children(CLUTTER_CONTAINER(stage));
    gint child_count = g_list_length(children);

    //Remove the children
    for(gint i = 0; i < child_count; ++i) {
        ClutterActor *a = CLUTTER_ACTOR(g_list_nth(children, i));
        clutter_container_remove_actor(CLUTTER_CONTAINER(stage), a);
    }

    float stage_width = clutter_actor_get_width(CLUTTER_ACTOR(stage));

    float margin = 10.0f;
    float spacing = 20.0f;

    float ypos = margin;

    for(TileArray::iterator iter = tiles_.begin(); iter != tiles_.end(); ++iter) {
        ClutterTexture* tex = (*iter)->get_texture();
        clutter_texture_set_keep_aspect_ratio(tex, true);
        clutter_actor_set_width(CLUTTER_ACTOR(tex), stage_width * 0.6f);

        float tex_height = clutter_actor_get_height(CLUTTER_ACTOR(tex));
        float tex_width = clutter_actor_get_width(CLUTTER_ACTOR(tex));

        if(ypos == margin) {
            ypos += (tex_height / 2.0f);
        }

        clutter_actor_set_position(CLUTTER_ACTOR(tex), (stage_width / 2.0f) - (tex_width / 2.0f), ypos - (tex_height / 2.0f));
        clutter_container_add_actor(CLUTTER_CONTAINER(stage), CLUTTER_ACTOR(tex));

        ypos += tex_height;
        ypos += spacing;
    }
}

/** @brief add_tile
  *
  * @todo: document this function
  */
void Tileset::add_tile(const std::string& path) {
    Tile::ptr new_tile(new Tile(path));
    tiles_.push_back(new_tile);
}

