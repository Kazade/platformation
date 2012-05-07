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


#ifndef OPENGL_WIDGET_H_INCLUDED
#define OPENGL_WIDGET_H_INCLUDED

#include "gtkgl/gtk_gl_widget.h"
#include <kazmath/vec2.h>

class OpenGLWidget : public GtkGLWidget {
public:
    OpenGLWidget(Gtk::DrawingArea* widget);
    void initialize();

    bool on_motion_notify(GdkEventMotion* event);
    bool on_button_press(GdkEventButton* event);
    bool on_button_release(GdkEventButton* event);
    bool on_scroll(GdkEventScroll* event);

private:
    virtual void do_button_press(GdkEventButton* event) {}
    virtual void do_motion(GdkEventMotion* event) {}
    virtual void do_button_release(GdkEventButton* event) {}
    virtual void do_scroll(GdkEventScroll* event) {}

protected:
    kmVec2 unproject(gdouble x, gdouble y);
};

#endif // OPENGL_WIDGET_H_INCLUDED
