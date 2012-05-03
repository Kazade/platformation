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


#include <gtkmm.h>
#include <gtkglmm.h>

#include "main_window.h"

int main(int argc, char* argv[]) {
    g_thread_init (NULL);

    Gtk::Main kit(argc, argv);
    Gtk::GL::init(argc, argv);

    MainWindow window;
    Gtk::Main::run(window.get_window_ref());
    return 0;
}
