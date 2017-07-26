//
// Created by bele on 26.07.17.
//

#ifndef BLUETOOTHCTL546_EXPERIMENTS_BLECONTROLLER_H
#define BLUETOOTHCTL546_EXPERIMENTS_BLECONTROLLER_H


#include <cstdint>
#include <list>

enum class BLEControllerUsage {
    Unused,
    Central,
    Peripheral
};

class BLEController {
public:
    const uint8_t *getMac() const;

    void setUsage(BLEControllerUsage usage);

    void freeUsage();

private:
    uint8_t mac[6] = {0};
    // used to prevent using a BLEController simultaniusly as a Central and Peripheral device
    BLEControllerUsage usage = BLEControllerUsage::Unused;
};


#endif //BLUETOOTHCTL546_EXPERIMENTS_BLECONTROLLER_H
