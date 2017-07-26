//
// Created by bele on 26.07.17.
//

#include "BLE.h"

std::shared_ptr<BLEController> BLE::getDefaultController() {
    std::shared_ptr<BLEController> controller = std::make_shared<BLEController>();
    return controller;
}

std::list<std::shared_ptr<BLEController>> BLE::getController() {
    std::list<std::shared_ptr<BLEController>> list;
    return list;
}

std::shared_ptr<BLEController> BLE::getController(std::string mac) {
    return std::shared_ptr<BLEController>();
}
