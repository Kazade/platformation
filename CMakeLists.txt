cmake_minimum_required(VERSION 2.6)

project(Platformation)

find_package(PkgConfig)
find_package(Boost COMPONENTS system filesystem signals REQUIRED)

pkg_check_modules(GTKMM REQUIRED gtkmm-2.4)
IF(GTKMM_FOUND)
    link_directories(${GTKMM_LIBRARY_DIRS})
    include_directories(${GTKMM_INCLUDE_DIRS})
ENDIF(GTKMM_FOUND)

pkg_check_modules(LIBGLADEMM REQUIRED libglademm-2.4)
IF(LIBGLADEMM_FOUND)
    link_directories(${LIBGLADEMM_LIBRARY_DIRS})
    include_directories(${LIBGLADEMM_INCLUDE_DIRS})
ENDIF(LIBGLADEMM_FOUND)

pkg_check_modules(CLUTTER REQUIRED clutter-1.0)
IF(CLUTTER_FOUND)
    link_directories(${CLUTTER_LIBRARY_DIRS})
    include_directories(${CLUTTER_INCLUDE_DIRS})
ENDIF(CLUTTER_FOUND)

pkg_check_modules(CLUTTERGTK REQUIRED clutter-gtk-0.10)
IF(CLUTTERGTK_FOUND)
    link_directories(${CLUTTERGTK_LIBRARY_DIRS})
    include_directories(${CLUTTERGTK_INCLUDE_DIRS})
ENDIF(CLUTTERGTK_FOUND)

pkg_check_modules(GTKGLEXTMM REQUIRED gtkglextmm-1.2)
IF(GTKGLEXTMM_FOUND)
    link_directories(${GTKGLEXTMM_LIBRARY_DIRS})
    include_directories(${GTKGLEXTMM_INCLUDE_DIRS})
ENDIF(GTKGLEXTMM_FOUND)

pkg_check_modules(RLOG REQUIRED librlog)
IF(RLOG_FOUND)
    link_directories(${RLOG_LIBRARY_DIRS})
    include_directories(${RLOG_INCLUDE_DIRS})
ENDIF(RLOG_FOUND)

set(sources
    src/main.cpp
    src/main_window.cpp
    src/new_level_dialog.cpp
    src/tileset.cpp
    src/level.cpp
    src/tile.cpp
    src/tile_instance.cpp
    src/opengl_widget.cpp
    src/editor_view.cpp
    src/opengl_grid.cpp
    src/opengl_tile_selector.cpp
    src/opengl_tile_editor.cpp
    src/geometry_element.cpp
    src/natural_sort.cpp
    src/layer.cpp
    src/layer_manager.cpp
)

set(headers
    src/main_window.h
    src/new_level_dialog.h
    src/tile.h
    src/tile_instance.h
    src/level.h
    src/opengl_widget.h
)

set(ui_files
    ui/main_window.glade
    ui/new_level_dialog.glade
)

set(CMAKE_CXX_FLAGS "-Wall")
add_executable(platformation ${sources})
target_link_libraries(platformation ${GTKMM_LIBRARIES} ${CLUTTER_LIBRARIES} ${CLUTTERGTK_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SIGNALS_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${GTKGLEXTMM_LIBRARIES} ${RLOG_LIBRARIES} /usr/lib/libSOIL.a /usr/lib/libtinyxml.a)

set(source_dir ${CMAKE_CURRENT_SOURCE_DIR}/ui)
set(destination_dir ${EXECUTABLE_OUTPUT_PATH}/ui)

add_custom_target(install_glade ALL ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/ui/
${CMAKE_CURRENT_BINARY_DIR}/ui/ )

add_dependencies(platformation install_glade)
