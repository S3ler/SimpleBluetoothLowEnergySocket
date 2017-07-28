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
#include "BLEScanner.h"
#include "BLEConnection.h"
#include "BLECentral.h"

void add_sigIntHandler();

void my_sigIntHandler(int s);

std::atomic<bool> sigIntReceived(false);

/*
 * Simple Bluetooth Low Energy Server
 * usage: BLEScan [XX:XX:XX:XX:XX:XX]
 * if no mac is given the default bluetooth adapter is used
 */
int main(int argc, char *argv[]) {

    BLE ble;
    std::list<std::shared_ptr<BLEController>> controllers = ble.getController();
    for (auto &&controller : controllers) {
        std::cout << "BLEController: " << controller->getMac() << std::endl;
    }

    std::shared_ptr<BLEController> controller;
    if (argc == 1) {
        controller = ble.getDefaultController();
        if (controller == nullptr) {
            std::cout << "Could not find a default bluetooth controller" << std::endl;
            exit(1);
        }
        // use default bluetooth adapter
    } else if (argc == 2) {
        std::string mac(argv[1]);
        controller = ble.getController(mac);
        if (controller == nullptr) {
            std::cout << "Could not find a bluetooth controller" << std::endl;
            exit(1);
        }
    }

    add_sigIntHandler();
    int flags = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, flags | O_NONBLOCK);

    BLEScanner bleScanner(controller);
    while (!sigIntReceived) {
        std::shared_ptr<BLEAdvertisment> bleAdvertisment = bleScanner.awaitAdvertisment();
        if (bleAdvertisment == nullptr) {
            continue;
        }
        std::cout << bleAdvertisment << std::endl;
    }
    bleScanner.stop();

    return 0;
}

void my_sigIntHandler(int s) {
    //https://stackoverflow.com/questions/1641182/how-can-i-catch-a-ctrl-c-event-c
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