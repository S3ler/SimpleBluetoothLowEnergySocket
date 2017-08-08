//
// Created by bele on 25.07.17.
//
#include <csignal>
#include <list>
#include <memory>
#include <atomic>
#include <iostream>
#include <fcntl.h>
#include <zconf.h>
#include "BLE.h"

int main(int argc, char *argv[]) {

    BLE ble;
    std::list<std::shared_ptr<BLEAdapter>> adapters = ble.getAdapters();
    if (adapters.empty()) {
        std::cout << "Could not find a bluetooth adapter" << std::endl;
        exit(1);
    }
    for (auto &&ad : adapters) {
        std::cout << "BLEAdapter: " << *(ad.get()) << std::endl;
    }

    return 0;
}
