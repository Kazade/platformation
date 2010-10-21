#ifndef CLUTTER_HELPER_H_INCLUDED
#define CLUTTER_HELPER_H_INCLUDED

#include <boost/function.hpp>

namespace clutter {
    namespace details {
        void signal_connect_callback( boost::function< void() > * real_callback ) {
            (*real_callback)();
        }
    }

    void signal_connect(ClutterActor* actor, gchar* event, boost::function< void () > &callback ) {
        g_signal_connect(actor, event, G_CALLBACK(&details::signal_connect_callback), &callback);
    }
}

#endif // CLUTTER_HELPER_H_INCLUDED
