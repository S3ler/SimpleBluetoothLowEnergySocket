//
// Created by bele on 26.07.17.
//

#include "BLEConnection.h"

bool BLEConnection::connect() {
    return false;
}

bool BLEConnection::isConnected() {
    return false;
}

std::string BLEConnection::getMessage() {
    return nullptr;
}

BLEConnection::BLEConnection(const uint8_t *mac) {

}

bool BLEConnection::send(std::string msg) {
    return false;
}

void BLEConnection::disconnect() {

}
