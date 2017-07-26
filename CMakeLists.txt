cmake_minimum_required(VERSION 3.7)
project(bluetoothctl546_experiments)

set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig REQUIRED)

# thank you: https://stackoverflow.com/questions/38515316/how-to-configure-dbus-dependencies-with-cmake
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(DBus REQUIRED)
include_directories(${DBUS_INCLUDE_DIRS})

pkg_search_module(GLIB REQUIRED glib-2.0)
include_directories(${GLIB_INCLUDE_DIRS})

include_directories(libBLE)

add_subdirectory(libBLE)
add_subdirectory(examples/client)
add_subdirectory(examples/server)


