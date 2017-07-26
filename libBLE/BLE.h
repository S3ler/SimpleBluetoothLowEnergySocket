//
// Created by bele on 26.07.17.
//

#ifndef BLUETOOTHCTL546_EXPERIMENTS_BLE_H
#define BLUETOOTHCTL546_EXPERIMENTS_BLE_H

#include <memory>
#include "BLEController.h"

class BLE {
public:
    std::shared_ptr<BLEController> getDefaultController();

    std::list<std::shared_ptr<BLEController>> getController();

    std::shared_ptr<BLEController> getController(std::string mac);
};


#endif //BLUETOOTHCTL546_EXPERIMENTS_BLE_H
