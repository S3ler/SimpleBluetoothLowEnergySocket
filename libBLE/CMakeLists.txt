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
        BLEAdvertisment.cpp
        BLEAdvertisment.h
        BLEController.cpp
        BLEController.h
        BLE.cpp
        BLE.h
        BLEConnection.cpp
        BLEConnection.h
        BLEPeripheral.cpp
        BLEPeripheral.h
        BLEScanner.cpp
        BLEScanner.h
        BLECentral.cpp BLECentral.h internal/scan.c)


add_library(libBLE ${SOURCE_FILES} BLEConnection.cpp BLEConnection.h)
target_link_libraries(libBLE ${DBUS_LIBRARIES} ${GLIB_LIBRARIES})