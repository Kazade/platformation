CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(platformation)

ADD_DEFINITIONS("-Wall -std=c++0x -g")

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")

FIND_PACKAGE(PkgConfig)
FIND_PACKAGE(Boost COMPONENTS system filesystem thread date_time regex REQUIRED)
FIND_PACKAGE(KGLT REQUIRED)
FIND_PACKAGE(KAZMATH REQUIRED)

pkg_check_modules(CURL REQUIRED libcurl)
pkg_check_modules(GTHREAD REQUIRED "gthread-2.0 >= 2.14.0")
pkg_check_modules(SIGC REQUIRED sigc++-2.0)
pkg_check_modules(GTKMM REQUIRED gtkmm-2.4)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4)
pkg_check_modules(GDKMM REQUIRED gtkmm-2.4)
pkg_check_modules(GL REQUIRED gl)
pkg_check_modules(GLU REQUIRED glu)

link_directories(
	${GL_LIBRARY_DIRS}
	${GLU_LIBRARY_DIRS}
)

include_directories(
	${GL_INCLUDE_DIRS}
	${GLU_INCLUDE_DIRS}
)

IF(GTKMM_FOUND)
    link_directories(${GTKMM_LIBRARY_DIRS})
    include_directories(${GTKMM_INCLUDE_DIRS})
ENDIF(GTKMM_FOUND)

link_directories(${GLIBMM_LIBRARY_DIRS} ${GDKMM_LIBRARY_DIRS})
include_directories(${GLIBMM_INCLUDE_DIRS} ${GDKMM_INCLUDE_DIRS})

pkg_check_modules(LIBGLADEMM REQUIRED libglademm-2.4)
IF(LIBGLADEMM_FOUND)
    link_directories(${LIBGLADEMM_LIBRARY_DIRS})
    include_directories(${LIBGLADEMM_INCLUDE_DIRS})
ENDIF(LIBGLADEMM_FOUND)

link_directories(${CURL_LIBRARY_DIRS})
include_directories(${CURL_INCLUDE_DIRS} ${SIGC_INCLUDE_DIRS})

FILE(GLOB_RECURSE sources platformation/*.cpp)
FILE(GLOB_RECURSE kazbase kazbase/*.cpp)

FOREACH(item ${kazbase})
  IF(${item} MATCHES "kazbase/tests.cpp")
    LIST(REMOVE_ITEM kazbase ${item})
  ENDIF(${item} MATCHES "kazbase/tests.cpp")
ENDFOREACH(item)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${KGLT_INCLUDE_DIRS}
    ${KAZMATH_INCLUDE_DIRS}
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
add_executable(platformation ${sources} ${kazbase})
target_link_libraries(
    platformation 
    ${KGLT_LIBRARIES}
    ${KAZMATH_LIBRARIES}
    ${GTKMM_LIBRARIES} 
    ${GTHREAD_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY} 
    ${Boost_REGEX_LIBRARY}
    ${Boost_THREAD_LIBRARY} 
    ${Boost_SYSTEM_LIBRARY} 
    ${Boost_DATE_TIME_LIBRARY}
    /usr/lib/libSOIL.a 
    /usr/lib/libtinyxml.a 
    ${CURL_LIBRARIES}
    ${GL_LIBRARIES}
    ${GLU_LIBRARIES}
)

set(source_dir ${CMAKE_CURRENT_SOURCE_DIR}/ui)
set(destination_dir ${EXECUTABLE_OUTPUT_PATH}/ui)

add_custom_target(install_glade ALL ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/ui/
${CMAKE_CURRENT_BINARY_DIR}/ui/ )

add_dependencies(platformation install_glade)
