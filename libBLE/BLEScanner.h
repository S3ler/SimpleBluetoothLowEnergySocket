//
// Created by bele on 25.07.17.
//

#ifndef BLUETOOTHCTL546_EXPERIMENTS_BLESCANNER_H
#define BLUETOOTHCTL546_EXPERIMENTS_BLESCANNER_H

#include <map>
#include <memory>
#include <atomic>
#include "BLEAdvertisment.h"
#include "BLEController.h"

class BLEScanner {

public:

    BLEScanner(std::shared_ptr<BLEController> shared_ptr);

    std::shared_ptr<BLEAdvertisment> awaitAdvertisment();

    std::shared_ptr<BLEAdvertisment> getAdvertisment();

    const std::atomic<bool> &isStopped() const;

    const std::atomic<bool> &isRunning() const;

    void scan();

    void stop();

private:

    std::atomic<bool> running;
    std::atomic<bool> stopped;
    std::map<std::string, std::shared_ptr<BLEAdvertisment>> bleAdvertisments;
    std::shared_ptr<BLEController> controller;
};


#endif //BLUETOOTHCTL546_EXPERIMENTS_BLESCANNER_H
