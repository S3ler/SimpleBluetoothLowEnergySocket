//
// Created by bele on 25.07.17.
//

#include <thread>
#include <utility>
#include "BLEScanner.h"

BLEScanner::BLEScanner(std::shared_ptr<BLEAdapter> controller) :
        controller(std::move(controller)){ }

std::shared_ptr<BLEAdvertisement> BLEScanner::getAdvertisment() {
    if (!controller->startScan(this)) {
        return nullptr;
    }
    return popAdvertisment();
}

void BLEScanner::pushAdvertisment(std::shared_ptr<BLEAdvertisement> advertisment){
    bleAdvertisments.insert(std::make_pair(advertisment->getMac(), advertisment));
    unrequestedBleAdvertisments.push(advertisment);
}

bool BLEScanner::stop() {
    return controller->stopScan();
}

std::shared_ptr<BLEAdvertisement> BLEScanner::popAdvertisment() {
    if(unrequestedBleAdvertisments.empty()){
        return nullptr;
    }
    std::shared_ptr<BLEAdvertisement> front;
    front = unrequestedBleAdvertisments.front();
    unrequestedBleAdvertisments.pop();
    return front;
}


BLEScanner::~BLEScanner() {
    if(controller != nullptr){
        controller->removeBLEScanner(this);
    }
}

void BLEScanner::notifyAdvertisment(std::shared_ptr<BLEAdvertisement> advertisment) {
    pushAdvertisment(advertisment);
}

