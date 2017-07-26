//
// Created by bele on 26.07.17.
//

#ifndef BLUETOOTHCTL546_EXPERIMENTS_BLECONNECTION_H
#define BLUETOOTHCTL546_EXPERIMENTS_BLECONNECTION_H


#include <cstdint>
#include <string>

class BLEConnection {
public:
    BLEConnection(const uint8_t *mac);

    bool connect();

    bool isConnected();

    std::string getMessage();

    bool send(std::string msg);

    void disconnect();
};


#endif //BLUETOOTHCTL546_EXPERIMENTS_BLECONNECTION_H
