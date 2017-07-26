cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 14)

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
        )


add_library(libBLEScan ${SOURCE_FILES} BLEConnection.cpp BLEConnection.h)
