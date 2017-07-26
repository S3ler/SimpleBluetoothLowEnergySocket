//
// Created by bele on 26.07.17.
//

#include "BLEController.h"

const uint8_t *BLEController::getMac() const {
    return mac;
}

void BLEController::setUsage(BLEControllerUsage usage) {
    // lock
    if(usage == BLEControllerUsage::Unused){
        // TODO throw exception
        // learn how to throw and catch exception proberly
    }
    if(BLEController::usage == BLEControllerUsage::Unused){
        BLEController::usage = usage;
    }
    if(BLEController::usage != usage){
        // TODO throw exception
        // learn how to throw and catch exception proberly
    }
}

void BLEController::freeUsage() {
    // lock
    BLEController::usage = BLEControllerUsage::Unused;
}
