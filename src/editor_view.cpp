#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>

#include "opengl_tile_selector.h"
#include "tile.h"
#include "object.h"
#include "level.h"
#include "editor_view.h"

/** @brief do_init
  *
  * @todo: document this function
  */
void EditorView::do_init()
{
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);
    glClearColor( 0.7, 0.7, 0.8, 1.0);
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

    float ytrans = 0.0f;
    float xtrans = 0.0f;
    if(Gtk::ScrolledWindow* scr = dynamic_cast<Gtk::ScrolledWindow*>(get_widget()->get_parent())) {
        ytrans = scr->get_vadjustment()->get_value();
        xtrans = scr->get_hadjustment()->get_value();
    }

    glTranslatef(0.0f, ytrans, 0.0f);

    grid_->render();

    if(level_) {
        Level::ObjectListIteratorPair iters = level_->get_iterators();

        for(; iters.first != iters.second; iters.first++) {
            Object* obj = (*iters.first).get();
            glBindTexture(GL_TEXTURE_2D, get_texture_for_object(obj));
            obj->render_geometry();
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
EditorView::EditorView(Gtk::DrawingArea* widget):
OpenGLWidget(widget),
level_(NULL),
active_object_(NULL),
tile_selector_(NULL)
{
    GridColour c;
    c.r = c.b = c.g = 0.5f;
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

    float zoom = 1.0f;

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
}

/** @brief do_button_press
  *
  * @todo: document this function
  */
void EditorView::do_button_press(GdkEventButton* event)
{
    if(event->button == 1) {
        MakeCurrent context(this);
        if(!context.ok) {
            return;
        }

        assert(picker_);

        gfloat x = event->x;
        gfloat y = event->y;

        Level::ObjectListIteratorPair iterators = level_->get_iterators();

        Object::ptr t = picker_->pick(x, y, iterators.first, iterators.second);
        if(t) {
            active_object_ = t.get();
            active_timer_.start();
        } else {
            active_object_ = NULL;
            active_timer_.reset();
        }
    } else if (event->button == 2 && tile_selector_) {
        Tile::id_type id = tile_selector_->get_active_tile_id();
        if(id != -1) {
            level_->spawn_tile_instance(id);
        }
    } else if (event->button == 3) {
        MakeCurrent context(this);
        if(!context.ok) {
            return;
        }

        gfloat x = event->x;
        gfloat y = event->y;

        Level::ObjectListIteratorPair iterators = level_->get_iterators();

        Object::ptr t = picker_->pick(x, y, iterators.first, iterators.second);

        if(t) {
            level_->delete_tile_instance(t.get());
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
        gdouble winx = event->x;
        gdouble winy = event->y;
        GLdouble winz = 0.0;

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
        Tile* t = ti->get_tile();
        assert(t);

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





