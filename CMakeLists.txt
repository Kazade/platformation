cmake_minimum_required(VERSION 2.6)

SET(CMAKE_VERBOSE_MAKEFILE ON)   

project(Platformation)

ADD_DEFINITIONS("-Wall -std=c++0x -g")

find_package(PkgConfig)
find_package(Boost COMPONENTS system filesystem signals thread date_time regex REQUIRED)

pkg_check_modules(CURL REQUIRED libcurl)

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

pkg_check_modules(GTKGLEXTMM REQUIRED gtkglextmm-1.2)
IF(GTKGLEXTMM_FOUND)
    message(STATUS ${GTKGLEXTMM_CFLAGS})
    link_directories(${GTKGLEXTMM_LIBRARY_DIRS})
    include_directories(${GTKGLEXTMM_INCLUDE_DIRS})
ENDIF(GTKGLEXTMM_FOUND)

link_directories(${CURL_LIBRARY_DIRS})
include_directories(${CURL_INCLUDE_DIRS})

FILE(GLOB_RECURSE sources platformation/*.cpp)
FILE(GLOB_RECURSE kazbase kazbase/*.cpp)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

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
add_executable(platformation ${sources} ${kazbase})
target_link_libraries(
    platformation 
    ${GTKMM_LIBRARIES} 
    ${Boost_FILESYSTEM_LIBRARY} 
    ${Boost_REGEX_LIBRARY}
    ${Boost_THREAD_LIBRARY} 
    ${Boost_SIGNALS_LIBRARY} 
    ${Boost_SYSTEM_LIBRARY} 
    ${GTKGLEXTMM_LIBRARIES} 
    /usr/lib/libSOIL.a 
    /usr/lib/libtinyxml.a 
    ${CURL_LIBRARIES}
)

set(source_dir ${CMAKE_CURRENT_SOURCE_DIR}/ui)
set(destination_dir ${EXECUTABLE_OUTPUT_PATH}/ui)

add_custom_target(install_glade ALL ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/ui/
${CMAKE_CURRENT_BINARY_DIR}/ui/ )

add_dependencies(platformation install_glade)
