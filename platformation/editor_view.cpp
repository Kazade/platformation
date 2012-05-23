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
#include "kazbase/logging/logging.h"

/** @brief do_init
  *
  * @todo: document this function
  */
void EditorView::do_init() {
    scene().render_options.backface_culling_enabled = false;
    scene().render_options.texture_enabled = true;
    scene().render_options.wireframe_enabled = false;
    scene().pass().viewport().set_background_colour(kglt::Colour(0.2078, 0.494, 0.78, 0.5));
    
    L_DEBUG("Initializing the editor view");    
}

/** @brief do_render
  *
  * @todo: document this function
  */
void EditorView::do_render() {
    update();
}

/** @brief EditorView
  *
  * @todo: document this function
  */
EditorView::EditorView(Gtk::DrawingArea* widget, MainWindow* parent):
OpenGLWidget(widget),
level_(NULL),
tile_selector_(NULL),
zoom_(1.0f),
parent_(parent),
selected_instance_(nullptr) {
	widget->set_flags(Gtk::CAN_FOCUS);
    widget->set_events(
		Gdk::POINTER_MOTION_MASK | 
		Gdk::BUTTON_PRESS_MASK |
		Gdk::ENTER_NOTIFY_MASK |
		Gdk::LEAVE_NOTIFY_MASK
	);
	
    widget->signal_motion_notify_event().connect(sigc::mem_fun(this, &EditorView::motion_notify_event));
    
    selection_.reset(new kglt::SelectionRenderer(scene()));
    
    scene().remove_all_passes();
    scene().add_pass(selection_);
    scene().add_pass(kglt::Renderer::ptr(new kglt::GenericRenderer(scene())));    
}


bool EditorView::motion_notify_event(GdkEventMotion* event) {
	if(event) {
		mouse_x_ = event->x;
		mouse_y_ = event->y;
	}
	return false;
}

/** @brief do_resize
  *
  * @todo: document this function
  */
void EditorView::do_resize(int width, int height) {
    set_width(area()->get_allocation().get_width());
    set_height(area()->get_allocation().get_height());
    
    scene().pass(0).renderer().set_orthographic_projection_from_height(8.0, float(width) / float(height));
    scene().pass(1).renderer().set_orthographic_projection_from_height(8.0, float(width) / float(height));
}

/** @brief set_level
  *
  * @todo: document this function
  */
void EditorView::set_level(Level* level)
{
    level_ = level;

    if(Gtk::ScrolledWindow* scr = dynamic_cast<Gtk::ScrolledWindow*>(get_widget()->get_parent())) {
        kmVec2 tl = unproject(0, 0);
        kmVec2 br = unproject(get_widget()->get_width(), get_widget()->get_height());
        float visible_width = br.x - tl.x;
        int lower = int(visible_width) / 2;
        scr->get_hadjustment()->set_lower(lower);
        scr->get_hadjustment()->set_value(lower);
        scr->get_hadjustment()->set_step_increment(1);
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

    if(!make_current()) return;
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
		if(selected_instance_) {
			selected_instance_->mark_selected(false);
		}
		
        //Select the tile instance under the cursor
		kglt::MeshID selected_mesh = selection_->selected_mesh();
		if(selected_mesh) {
			kglt::Mesh& mesh = scene().mesh(selected_mesh);
			TileInstance* instance = (TileInstance*) mesh.user_data();
			if(instance) {
				selected_instance_ = instance;
				selected_instance_->mark_selected();
			} else {
				selected_instance_ = nullptr;
			}
		} else {
			selected_instance_ = nullptr;
		}

/*        assert(picker_);

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
        }*/

    } else if (event->button == 2 && tile_selector_) {
/*        //Spawn a new tile instance

        //Get the spawn position (where we clicked
        if(!make_current()) return;
        
        kmVec2 pos = unproject(event->x, event->y);
        //Snap the position to the grid
        grid_->snap_to(pos.x, pos.y);

        //Run the action, if it does anything we should get a new instance
        SpawnTileInstanceAction::ptr action(new SpawnTileInstanceAction(level_, tile_selector_, pos.x, pos.y));
        action->do_action();
        TileInstance* ni = action->get_spawned_instance();
        if(ni) {
            parent_->get_action_manager().push_action(action); //Add to the undo queue, but only if the action did something
        }*/
    } else if (event->button == 3) {
        //Delete the instance under the cursor
/*        if(!make_current()) return;

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

        }*/
    }
}

/** @brief do_motion
  *
  * @todo: document this function
  */
void EditorView::do_motion(GdkEventMotion* event) {

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





