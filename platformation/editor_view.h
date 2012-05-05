#ifndef HEADER_82F5ABE51BFF30B2
#define HEADER_82F5ABE51BFF30B2

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


#ifndef EDITOR_VIEW_H_INCLUDED
#define EDITOR_VIEW_H_INCLUDED

#include <glibmm.h>

#include <boost/shared_ptr.hpp>
#include "opengl_widget.h"
#include "opengl_grid.h"
#include "opengl_picker.h"

class Level;
class Object;
class MainWindow;
class OpenGLTileSelector;

class EditorView : public OpenGLWidget {
public:
    typedef std::tr1::shared_ptr<EditorView> ptr;

    EditorView(Gtk::DrawingArea* widget, MainWindow* parent);

    void set_level(Level* level);
    void set_tile_selector(OpenGLTileSelector* selector);
    
    GLuint get_texture_for_object(Object* obj);
    
private:
    MainWindow* parent_;

    void do_render();
    void do_init();
    void do_resize(int width, int height);
    void do_button_press(GdkEventButton* event);
    void do_motion(GdkEventMotion* event);
    void do_scroll(GdkEventScroll* event);

    OpenGLGrid::ptr grid_;
    Level* level_;

    OpenGLPicker<Object::ptr>::ptr picker_;
    Object* active_object_;
    Glib::Timer active_timer_;
    std::map<Object*, GLuint> texture_ids_;
    OpenGLTileSelector* tile_selector_;

    float zoom_;
};


#endif // EDITOR_VIEW_H_INCLUDED

#endif // header guard
