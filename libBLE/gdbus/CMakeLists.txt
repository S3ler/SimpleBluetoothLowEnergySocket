cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 14)


find_package(PkgConfig REQUIRED)

# thank you: https://stackoverflow.com/questions/38515316/how-to-configure-dbus-dependencies-with-cmake
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(DBus REQUIRED)
include_directories(${DBUS_INCLUDE_DIRS})

pkg_search_module(GLIB REQUIRED glib-2.0)
include_directories(${GLIB_INCLUDE_DIRS})

set(SOURCE_FILES
        client.c
        gdbus.h
        mainloop.c
        object.c
        polkit.c
        watch.c
        )


add_library(libgdbus ${SOURCE_FILES} )
target_link_libraries(libgdbus ${DBUS_LIBRARIES} ${GLIB_LIBRARIES})