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


#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>

#include "main_window.h"
#include "opengl_tile_selector.h"
#include "tile.h"
#include "object.h"
#include "level.h"
#include "editor_view.h"
#include "actions/spawn_tile_instance_action.h"

/** @brief do_init
  *
  * @todo: document this function
  */
void EditorView::do_init()
{
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);
    glAlphaFunc(GL_GREATER, 0.1f);
    glEnable(GL_ALPHA_TEST);
    glClearColor(0.2078, 0.494, 0.78, 0.5);
}

/** @brief do_render
  *
  * @todo: document this function
  */
void EditorView::do_render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);

    float ytrans = 0.0f;
    float xtrans = 0.0f;
    if(Gtk::ScrolledWindow* scr = dynamic_cast<Gtk::ScrolledWindow*>(get_widget()->get_parent())) {
        ytrans = scr->get_vadjustment()->get_value();
        xtrans = scr->get_hadjustment()->get_value();
    }

    glTranslatef(-xtrans, ytrans, 0.0f);

    if(level_) {
        grid_->render();
    }

    if(level_) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);
        for(uint32_t i = 0; i < level_->layer_count(); ++i) {
            LayerID layer = level_->layer_by_index(i);

            if(layer != level_->active_layer_id()) {
                glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
            } else {
                glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            }

            level_->render_layer(*this, layer);
        }

        if(active_object_) {
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, -0.1f);
            glLineWidth(2);
            glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_POLYGON_BIT);
                glColor3f(1.0f, 0.0f, 0.0f);
                glDisable(GL_TEXTURE_2D);
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                active_object_->render_geometry();
            glPopAttrib();
            glPopMatrix();
        }
    }
}

/** @brief EditorView
  *
  * @todo: document this function
  */
EditorView::EditorView(Gtk::DrawingArea* widget, MainWindow* parent):
OpenGLWidget(widget),
level_(NULL),
active_object_(NULL),
tile_selector_(NULL),
zoom_(1.0f),
parent_(parent)
{
    GridColour c;
    c.r = c.b = c.g = 1.0f;
    grid_.reset(new OpenGLGrid(1.0f, 4, c));
    picker_.reset(new OpenGLPicker<Object::ptr>());
}

/** @brief do_resize
  *
  * @todo: document this function
  */
void EditorView::do_resize(int width, int height)
{
    glViewport (0, 0, (GLfloat)width, (GLfloat)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float zoom = zoom_;

    float num_tiles_across = 40.0f;

    float left = zoom * -(num_tiles_across / 2.0f);
    float right = zoom * (num_tiles_across / 2.0f);

    float ratio = float(height) / float(width);

    float num_tiles_up = num_tiles_across * ratio;

    float top = zoom * (num_tiles_up / 2.0f);
    float bottom = zoom * -(num_tiles_up / 2.0f);

    glOrtho(left, right, bottom, top, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/** @brief set_level
  *
  * @todo: document this function
  */
void EditorView::set_level(Level* level)
{
    level_ = level;

    std::pair<int, int> level_size = level_->get_level_size();
    grid_->set_max_x(level_size.first / 64);
    grid_->set_min_x(0);
    grid_->set_max_y(level_size.second / 64 / 2);
    grid_->set_min_y(-level_size.second / 64 / 2);

    if(Gtk::ScrolledWindow* scr = dynamic_cast<Gtk::ScrolledWindow*>(get_widget()->get_parent())) {
        kmVec2 tl = unproject(0, 0);
        kmVec2 br = unproject(get_widget()->get_width(), get_widget()->get_height());
        float visible_width = br.x - tl.x;
        int lower = int(visible_width) / 2;
        scr->get_hadjustment()->set_lower(lower);
        scr->get_hadjustment()->set_value(lower);
        scr->get_hadjustment()->set_step_increment(1);
        scr->get_hadjustment()->set_upper(level_size.first / 64); //FIXME: This is wrong, need to correctly calculate max
    }
}

/** @brief do_scroll
  *
  * @todo: document this function
  */
void EditorView::do_scroll(GdkEventScroll* event)
{
    if(event->direction == GDK_SCROLL_UP) {
        zoom_ -= 0.2;
    } else {
        zoom_ += 0.2;
    }

    if(zoom_ <= 0.2f) zoom_ = 0.2f;

    MakeCurrent context(this);
    do_resize(get_widget()->get_width(), get_widget()->get_height());
}



/** @brief do_button_press
  *
  * @todo: document this function
  */
void EditorView::do_button_press(GdkEventButton* event)
{
    if(!level_) {
        return;
    }

    if(event->button == 1) {
        //Select the tile instance under the cursor
        MakeCurrent context(this);
        if(!context.ok) {
            return;
        }

        assert(picker_);

        gfloat x = event->x;
        gfloat y = event->y;

        Layer& layer = level_->active_layer();
        Layer::TileListIteratorPair iterators = layer.get_iterators();

        Object::ptr t = picker_->pick(x, y, iterators.first, iterators.second);
        if(t) {
            active_object_ = t.get();
            active_timer_.start();
        } else {
            active_object_ = NULL;
            active_timer_.reset();
        }

    } else if (event->button == 2 && tile_selector_) {
        //Spawn a new tile instance

        //Get the spawn position (where we clicked
        MakeCurrent context(this);
        kmVec2 pos = unproject(event->x, event->y);
        //Snap the position to the grid
        grid_->snap_to(pos.x, pos.y);

        //Run the action, if it does anything we should get a new instance
        SpawnTileInstanceAction::ptr action(new SpawnTileInstanceAction(level_, tile_selector_, pos.x, pos.y));
        action->do_action();
        TileInstance* ni = action->get_spawned_instance();
        if(ni) {
            parent_->get_action_manager().push_action(action); //Add to the undo queue, but only if the action did something
        }
    } else if (event->button == 3) {
        //Delete the instance under the cursor
        MakeCurrent context(this);
        if(!context.ok) {
            return;
        }

        gfloat x = event->x;
        gfloat y = event->y;

        Layer& layer = level_->active_layer();
        Layer::TileListIteratorPair iterators = layer.get_iterators();

        Object::ptr t = picker_->pick(x, y, iterators.first, iterators.second);

        if(t) {
            TileInstance* ti = dynamic_cast<TileInstance*>(t.get());
            if(ti) {
                level_->delete_tile_instance(ti);
                active_object_ = NULL;
            }

        }
    }
}

/** @brief do_motion
  *
  * @todo: document this function
  */
void EditorView::do_motion(GdkEventMotion* event)
{
    /*
        If we are moving, have an active object, and the mouse button
        is still down, we are trying to move it somewhere.
    */

    if(active_object_ && event->state & GDK_BUTTON1_MASK && active_timer_.elapsed() >= 0.2) {
        MakeCurrent context(this);
        if(!context.ok) {
            return;
        }

        kmVec2 pos = unproject(event->x, event->y);
        grid_->snap_to(pos.x, pos.y);
        active_object_->set_position(pos.x, pos.y);
    }
}

/** @brief get_texture_for_object
  *
  * @todo: document this function
  */
GLuint EditorView::get_texture_for_object(Object* obj)
{
    if(texture_ids_.find(obj) != texture_ids_.end()) {
        return texture_ids_[obj];
    }

    if(TileInstance* ti = dynamic_cast<TileInstance*>(obj)) {
        TileID tile_id = ti->tile_id();
        Tile* t = &level_->tile(tile_id);

        GLuint new_tex;
        glGenTextures(1, &new_tex);
        glBindTexture(GL_TEXTURE_2D, new_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, (t->get_channels() == 4) ? GL_RGBA : GL_RGB, t->get_width(), t->get_height(), 0, (t->get_channels() == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, t->get_data());
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        texture_ids_[obj] = new_tex;
        return new_tex;
    }

    assert(0 && "Not implemented");
}


/** @brief set_tile_selector
  *
  * @todo: document this function
  */
void EditorView::set_tile_selector(OpenGLTileSelector* selector)
{
    tile_selector_ = selector;
}





