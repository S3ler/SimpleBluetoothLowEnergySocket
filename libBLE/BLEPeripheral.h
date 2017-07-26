//
// Created by bele on 26.07.17.
//

#ifndef BLUETOOTHCTL546_EXPERIMENTS_BLEPERIPHERAL_H
#define BLUETOOTHCTL546_EXPERIMENTS_BLEPERIPHERAL_H


#include <memory>
#include <atomic>
#include "BLEController.h"
#include "BLEConnection.h"

class BLEPeripheral {
public:
    BLEPeripheral(std::shared_ptr <BLEController> controller);

    bool advertise();

    const std::atomic<bool> &isConnected() const;

    std::shared_ptr<BLEConnection> awaitConnection() const;


private:
    std::atomic<bool> connected;
    std::shared_ptr <BLEController> controller;
};


#endif //BLUETOOTHCTL546_EXPERIMENTS_BLEPERIPHERAL_H
