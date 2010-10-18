#include <gtkmm.h>

#include "main_window.h"

int main(int argc, char* argv[]) {
    g_thread_init (NULL);
    clutter_init (&argc, &argv);

    Gtk::Main kit(argc, argv);

    MainWindow window;
    Gtk::Main::run(window.get_window_ref());
    return 0;
}
