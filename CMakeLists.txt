cmake_minimum_required(VERSION 2.6)

project(gnova_edit)

find_package(PkgConfig)

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

find_package(Boost COMPONENTS system filesystem REQUIRED)


set(sources
    src/main.cpp
    src/main_window.cpp
    src/new_level_dialog.cpp
    src/tileset.cpp
    src/level.cpp
)

set(headers
    src/main_window.h
    src/new_level_dialog.h
)

set(ui_files
    ui/main_window.glade
    ui/new_level_dialog.glade
)

add_executable(gnovaedit ${sources})
target_link_libraries(gnovaedit ${GTKMM_LIBRARIES} ${CLUTTER_LIBRARIES} ${CLUTTERGTK_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})

set(source_dir ${CMAKE_CURRENT_SOURCE_DIR}/ui)
set(destination_dir ${EXECUTABLE_OUTPUT_PATH}/ui)

add_custom_target(install_glade ALL ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/ui/
${CMAKE_CURRENT_BINARY_DIR}/ui/ )

add_dependencies(gnovaedit install_glade)
