//
// Created by bele on 25.07.17.
//

#ifndef BLUETOOTHCTL546_EXPERIMENTS_BLEADVERTISMENT_H
#define BLUETOOTHCTL546_EXPERIMENTS_BLEADVERTISMENT_H

#include "BLEDevice.h"

#include <cstdint>
#include <ostream>
#include <chrono>
#include <gdbus/gdbus.h>
#include <memory>

class BLEDevice;

class BLEAdvertisement {
public:
    BLEAdvertisement(const std::string &mac, const std::string &name, const std::string &alias, int16_t rssi,
                     int16_t txPower);

    explicit BLEAdvertisement(std::shared_ptr<BLEDevice> bleDevice);

    const std::string &getMac() const;

    const std::string &getName() const;

    const std::string &getAlias() const;

    int16_t getRssi() const;

    int16_t getTxPower() const;

    time_t getTimestamp() const;

    friend std::ostream &operator<<(std::ostream &os, const BLEAdvertisement &advertisement);

private:
    std::string mac;
    std::string name;
    std::string alias;
    int16_t rssi;
    int16_t txPower;
    time_t timestamp;

};


#endif //BLUETOOTHCTL546_EXPERIMENTS_BLEADVERTISMENT_H
