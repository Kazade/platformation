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

#include "kglt/window_base.h"
#include "kglt/rendering/selection_renderer.h"

class Level;
class Object;
class MainWindow;
class OpenGLTileSelector;

class EditorView : public OpenGLWidget, public kglt::WindowBase {
public:
    typedef std::tr1::shared_ptr<EditorView> ptr;

    EditorView(Gtk::DrawingArea* widget, MainWindow* parent);

    void set_level(Level* level);
    void set_tile_selector(OpenGLTileSelector* selector);
    
    GLuint get_texture_for_object(Object* obj);
    
    //KGLT overrides
    virtual void set_title(const std::string& title) {}
    virtual void check_events() {}
    virtual void swap_buffers() {
        swap_gl_buffers();
    }
    
    virtual sigc::signal<void, kglt::KeyCode>& signal_key_pressed() { assert(0); }
    virtual sigc::signal<void, kglt::KeyCode>& signal_key_released() { assert(0); }

    virtual void cursor_position(int32_t& mouse_x, int32_t& mouse_y) {
		mouse_x = mouse_x_;
		mouse_y = mouse_y_;
		area()->get_pointer(mouse_x, mouse_y);
		std::cout << mouse_x << ", " << mouse_y << std::endl;
	}
	
	bool motion_notify_event(GdkEventMotion* event);
private:
    MainWindow* parent_;

    void do_render();
    void do_init();
    void do_resize(int width, int height);
    void do_button_press(GdkEventButton* event);
    void do_motion(GdkEventMotion* event);
    void do_scroll(GdkEventScroll* event);

    Level* level_;
    Glib::Timer active_timer_;
    std::map<Object*, GLuint> texture_ids_;
    OpenGLTileSelector* tile_selector_;

    float zoom_;
    
    int32_t mouse_x_;
    int32_t mouse_y_;
    
    kglt::SelectionRenderer::ptr selection_;
    TileInstance* selected_instance_;
};


#endif // EDITOR_VIEW_H_INCLUDED

#endif // header guard
