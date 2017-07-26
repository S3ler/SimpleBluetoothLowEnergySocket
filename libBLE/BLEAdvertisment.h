//
// Created by bele on 25.07.17.
//

#ifndef BLUETOOTHCTL546_EXPERIMENTS_BLEADVERTISMENT_H
#define BLUETOOTHCTL546_EXPERIMENTS_BLEADVERTISMENT_H


#include <cstdint>
#include <ostream>

class BLEAdvertisment {
public:

    BLEAdvertisment(uint8_t *mac);

    const uint8_t *getMac() const;

    bool isConnectable() const;

    friend std::ostream &operator<<(std::ostream &os, const BLEAdvertisment &advertisment);

private:
    uint8_t mac[6] = {0};
    bool connectable = false;
};


#endif //BLUETOOTHCTL546_EXPERIMENTS_BLEADVERTISMENT_H
