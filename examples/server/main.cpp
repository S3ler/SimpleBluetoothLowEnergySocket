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
#include "BLENUSConnection.h"
#include "BLECentral.h"

void add_sigIntHandler();

void my_sigIntHandler(int s);

std::atomic<bool> sigIntReceived(false);

/*
 * Simple Bluetooth Low Energy Server
 */
int main(int argc, char *argv[]) {

    BLE ble;
    std::list<std::shared_ptr<BLEAdapter>> adapters = ble.getAdapters();
    if (adapters.empty()) {
        std::cout << "Could not find a bluetooth controller" << std::endl;
        exit(1);
    }
    for (auto &&ad : adapters) {
        std::cout << "BLEAdapter: " << ad->getMac() << std::endl;
    }

    std::shared_ptr<BLEAdapter> adapter;
    if (argc == 1) {
        adapter = adapters.back();
        if (adapter == nullptr) {
            std::cout << "Could not find a default bluetooth controller" << std::endl;
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
            std::cout << "Could not find a bluetooth controller" << std::endl;
            exit(1);
        }
    }

    add_sigIntHandler();
    int flags = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, flags | O_NONBLOCK);

    BLECServer bleCentral(adapters);
    while (!sigIntReceived) {
        std::shared_ptr<BLEConnection> bleConnection = bleCentral.awaitConnection();
        if (bleConnection == nullptr) {
            continue;
        }
        if (bleConnection->connect()) {
            while (bleConnection->isConnected() && !sigIntReceived) {
                char input_buffer[255] = {0};
                if (read(0, input_buffer, 255) == -1) {
                    std::string msg = bleConnection->getMessage();
                    if (msg.empty()) {
                        std::cout << msg << std::endl;
                    }
                    continue;
                }
                std::string input(input_buffer);

                if (bleConnection->send(input)) {
                    std::cout << "send success" << std::endl;
                } else {
                    std::cout << "send failure" << std::endl;
                }

            }
            bleConnection->disconnect();
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