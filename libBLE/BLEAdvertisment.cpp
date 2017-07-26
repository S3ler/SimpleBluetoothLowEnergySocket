//
// Created by bele on 25.07.17.
//

#include "BLEAdvertisment.h"

BLEAdvertisment::BLEAdvertisment(uint8_t *mac) : mac{mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]} {}

std::ostream &operator<<(std::ostream &os, const BLEAdvertisment &advertisment) {
    os << "mac: " << advertisment.mac;
    return os;
}

const uint8_t *BLEAdvertisment::getMac() const {
    return mac;
}

bool BLEAdvertisment::isConnectable() const {
    return connectable;
}


