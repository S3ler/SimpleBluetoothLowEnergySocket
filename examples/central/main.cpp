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
#include <BLEAdapterRemovedException.h>
#include "BLE.h"
#include "BLENUSConnection.h"

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
        adapter = adapters.front();
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
    std::cout << "using: " << *adapter << std::endl;

    add_sigIntHandler();
    int flags = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, flags | O_NONBLOCK);

    adapter->startScan();

    while (!sigIntReceived) {
        if(adapter == nullptr){
            adapters = ble.getAdapters();
            if(adapters.empty()){
                continue;
            }
            adapter = adapters.front();
            continue;
        }
        std::list<std::shared_ptr<BLEDevice>> bleDevices;
        try {
            bleDevices = adapter->getDevices();
        }catch  (const BLEAdapterRemovedException& e){
            adapter = nullptr;
            std::cout << "Adapter Removed" << std::endl;
            continue;
        }

        //bleDevices = adapter->getDevices();
        if(bleDevices.empty()){
            continue;
        }
        for (auto &&d : bleDevices) {
            std::cout << *d << std::endl;
        }
        std::shared_ptr<BLEDevice> bleDevice = bleDevices.front();

        if (bleDevice->connect()) {
            std::cout << *bleDevice << std::endl;
            if(!bleDevice->hasNUS()){
                bleDevice->disconnect();
                continue;
            }
            std::shared_ptr<BLENUSConnection> bleNUSConnection= bleDevice->getNUSConnection();
            bleNUSConnection->connect();
            std::cout << "Connected with NUS: " << *bleDevice << std::endl;
            while (bleDevice->isConnected() && !sigIntReceived) {
                char input_buffer[255] = {0};
                if (read(0, input_buffer, 255) == -1) {
                    std::vector<uint8_t> msg = bleNUSConnection->getMessage();
                    if (!msg.empty()) {
                        std::string str(msg.begin(), msg.end());
                        std::cout << "Received Message:" << str << std::flush;
                    }
                    continue;
                }
                std::string input(input_buffer);
                if(!input.empty()){
                    std::vector<uint8_t> msg;
                    std::copy(input.begin(),input.end(), std::back_inserter(msg));
                    msg.push_back('\0');
                    if(msg.size()>20){
                        std::cout << "Cannot send message longer than 20 bytes." << std::endl;
                        continue;
                    }
                    if (bleNUSConnection->send(msg)) {
                        std::cout << "Send Success" << std::endl;
                    } else {
                        std::cout << "Send Failure" << std::endl;
                    }
                }
            }
            if(!bleDevice->isConnected()){
                bleNUSConnection->disconnect();
                bleDevice->disconnect();
            }
        }
        if(sigIntReceived){
            break;
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