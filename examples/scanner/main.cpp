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
#include <iomanip>
#include "BLE.h"


void add_sigIntHandler();

void my_sigIntHandler(int s);

std::atomic<bool> sigIntReceived(false);

/*
 * Simple Bluetooth Low Energy Scanner
 */
int main(int argc, char *argv[]) {

    BLE ble;
    std::list<std::shared_ptr<BLEAdapter>> adapters = ble.getAdapters();
    if (adapters.empty()) {
        std::cout << "Could not find a bluetooth adapter" << std::endl;
        exit(1);
    }
    std::cout << "available adapter: " << std::endl;
    for (auto &&ad : adapters) {
        std::cout << "BLEAdapter: " << ad->getMac() << std::endl;
    }

    std::shared_ptr<BLEAdapter> adapter;
    if (argc == 1) {
        adapter = adapters.back();
        if (adapter == nullptr) {
            std::cout << "Could not find a default bluetooth adapter" << std::endl;
            exit(1);
        }
        // use default bluetooth adapter
    } else if (argc == 2) {
        std::string mac(argv[1]);
        for (auto &&a : adapters) {
            if (!a->getMac().compare(mac)) {
                adapter = a;
                break;
            }
        }
        if (adapter == nullptr) {
            std::cout << "Could not find a bluetooth adapter" << std::endl;
            exit(1);
        }
    }
    std::cout << "using adapter: " << adapter->getMac() << std::endl;


    add_sigIntHandler();
    int flags = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, flags | O_NONBLOCK);
    BLEScanner bleScanner(adapter);
    while (!sigIntReceived) {
        std::shared_ptr<BLEAdvertisement> bleAdvertisment = bleScanner.getAdvertisment();
        if (bleAdvertisment == nullptr) {
            continue;
        }
        auto ad_time =bleAdvertisment->getTimestamp();
        std::cout << std::put_time(std::localtime(&ad_time), "%Y-%m-%d %X")
                  << " BLEAdvertisement: " << *(bleAdvertisment.get()) << std::endl;
    }
    bleScanner.stop();

    return 0;
}

void my_sigIntHandler(int s) {
    // https://stackoverflow.com/questions/1641182/how-can-i-catch-a-ctrl-c-event-c
    std::cout << std::endl;
    sigIntReceived = true;
}

void add_sigIntHandler() {
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = my_sigIntHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

}