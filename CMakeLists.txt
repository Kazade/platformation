cmake_minimum_required(VERSION 2.6)

project(gnova_edit)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-2.4)
pkg_check_modules(CLUTTER clutter-gtk-0.10)

link_directories( ${GTKMM_LIBRARY_DIRS} )
include_directories( ${GTKMM_INCLUDE_DIRS} ${CLUTTER_GTK_CFLAGS})

set(sources
    src/main.cpp
    src/main_window.cpp
)

set(headers
    src/main_window.h
)

set(ui_files
    ui/main_window.glade
)

add_executable(gnovaedit ${sources})
target_link_libraries(gnovaedit ${GTKMM_LIBRARIES})

set(source_dir ${CMAKE_CURRENT_SOURCE_DIR}/ui)
set(destination_dir ${EXECUTABLE_OUTPUT_PATH}/ui)

add_custom_target(install_glade ALL ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/ui/
${CMAKE_CURRENT_BINARY_DIR}/ui/ )

add_dependencies(gnovaedit install_glade)
