cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 14)


find_package(PkgConfig REQUIRED)

# thank you: https://stackoverflow.com/questions/38515316/how-to-configure-dbus-dependencies-with-cmake
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(DBus REQUIRED)
include_directories(${DBUS_INCLUDE_DIRS})

pkg_search_module(GLIB REQUIRED glib-2.0)
include_directories(${GLIB_INCLUDE_DIRS})

add_subdirectory(gdbus)
include_directories(proxy/internal)
include_directories(uuid)

set(SOURCE_FILES

        # high level object
        BLEScanner.h
        BLEScanner.cpp
        BLEAdvertisement.h
        BLEAdvertisement.cpp
        BLENUSConnection.cpp
        BLENUSConnection.h

        # ble api
        BLE.cpp
        BLE.h
        BLEAdapter.h
        BLEAdapter.cpp
        BLEDevice.cpp
        BLEDevice.h
        BLEGattService.cpp
        BLEGattService.h
        BLEGattCharacteristic.cpp
        BLEGattCharacteristic.h
        BLEGattDescriptor.cpp
        BLEGattDescriptor.h

        # proxy
        proxy/Proxy.h
        proxy/Proxy.cpp
        proxy/AdapterProxy.cpp
        proxy/AdapterProxy.h
        proxy/DeviceProxy.cpp
        proxy/DeviceProxy.h
        proxy/ServiceProxy.cpp
        proxy/ServiceProxy.h
        proxy/CharacteristicProxy.cpp
        proxy/CharacteristicProxy.h
        proxy/DescriptorProxy.cpp
        proxy/DescriptorProxy.h

        # dbus
        proxy/dbus/BLEDBusConnection.cpp
        proxy/dbus/BLEDBusConnection.h
        proxy/dbus/getsetdbusvalue.cpp
        proxy/dbus/StartDiscovery.cpp
        proxy/dbus/StartDiscovery.h
        proxy/dbus/getsetdbusvalue.h
        proxy/dbus/ConnectDevice.cpp
        proxy/dbus/ConnectDevice.h
        proxy/dbus/DisconnectDevice.cpp
        proxy/dbus/DisconnectDevice.h
        proxy/dbus/parserrorname.cpp
        proxy/dbus/parserrorname.h

        proxy/AdapterEnums.h
        proxy/CharacteristicEnum.h
        proxy/DescriptorEnum.h
        proxy/ServiceEnum.h
        proxy/DeviceProperty.h

        # helper
        enum.h
        infix_iterator.h
        safe_flag.h
        helper/Observer.h
        helper/ValueObserver.h
        proxy/dbus/StartNotify.cpp proxy/dbus/StartNotify.h proxy/dbus/StopNotify.cpp proxy/dbus/StopNotify.h proxy/dbus/WriteValue.cpp proxy/dbus/WriteValue.h BLEAdapterRemovedException.cpp BLEAdapterRemovedException.h proxy/dbus/RemoveDevice.cpp proxy/dbus/RemoveDevice.h)


add_library(libBLE ${SOURCE_FILES})
target_link_libraries(libBLE ${DBUS_LIBRARIES} ${GLIB_LIBRARIES} libgdbus pthread)