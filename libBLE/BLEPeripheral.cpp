//
// Created by bele on 26.07.17.
//

#include "BLEPeripheral.h"

BLEPeripheral::BLEPeripheral(std::shared_ptr<BLEController> controller) : controller(std::move(controller)) {
    connected = false;
}

const std::atomic<bool> &BLEPeripheral::isConnected() const {
    return connected;
}

bool BLEPeripheral::advertise() {
    return false;
}

std::shared_ptr<BLEConnection> BLEPeripheral::awaitConnection() const {
    const uint8_t fakemake[2] = {0};
    return std::make_shared<BLEConnection>(fakemake);
}

