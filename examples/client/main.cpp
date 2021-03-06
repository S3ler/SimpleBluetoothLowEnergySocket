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
#include <BLEAdapter.h>
#include <BLE.h>
#include "BLEPeripheral.h"

void add_sigIntHandler();

void my_sigIntHandler(int s);

std::atomic<bool> sigIntReceived(false);

/*
 * Simple Low Energy Client
 * usage: BLEPeripheral [XX:XX:XX:XX:XX:XX]
 * if no mac is given the default bluetooth adapter is used
 */
int main(int argc, char *argv[]) {
    BLE ble;

    std::list<std::shared_ptr<BLEAdapter>> controllers = ble.getAdapters();
    for (auto &&controller : controllers) {
        std::cout << "BLEAdapter: " << controller->getMac() << std::endl;
    }

    std::shared_ptr<BLEAdapter> controller;
    if (argc == 1) {
        controller = ble.getDefaultAdapter();
        if (controller == nullptr) {
            std::cout << "Could not find a default bluetooth controller" << std::endl;
            exit(1);
        }
        // use default bluetooth adapter
    } else if (argc == 2) {
        std::string mac(argv[1]);
        controller = ble.getAdapter(mac);
        if (controller == nullptr) {
            std::cout << "Could not find a bluetooth controller" << std::endl;
            exit(1);
        }
    }

    add_sigIntHandler();
    int flags = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, flags | O_NONBLOCK);

    BLEPeripheral peripheral(controller);
    while (!sigIntReceived) {
        if (peripheral.advertise()) {
            std::shared_ptr<BLEConnection> connection = peripheral.awaitConnection();
            if (connection == nullptr) {
                continue;
            }
            while (peripheral.isConnected() && !sigIntReceived) {
                char input_buffer[255] = {0};
                if (read(0, input_buffer, 255) == -1) {
                    std::string msg = connection->getMessage();
                    if (msg.empty()) {
                        std::cout << msg << std::endl;
                    }
                    continue;
                }
                std::string input(input_buffer);

                if (connection->send(input)) {
                    std::cout << "send success" << std::endl;
                } else {
                    std::cout << "send failure" << std::endl;
                }

            }
            connection->disconnect();
        }
    }

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