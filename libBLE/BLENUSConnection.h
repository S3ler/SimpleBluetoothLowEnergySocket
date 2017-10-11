//
// Created by bele on 06.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_BLECONNECTION_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_BLECONNECTION_H

#include "BLEDevice.h"
#include "BLEGattService.h"
#include "BLEGattCharacteristic.h"
#include "BLEGattDescriptor.h"
#include <cstdint>
#include <vector>
#include <helper/Observer.h>
#include <queue>
#include <proxy/CharacteristicEnum.h>
#include <helper/ValueObserver.h>

class BLEGattService;
// TODO make this class nullptr save
class BLENUSConnection : public ValueObserver {
public:
    BLENUSConnection(const std::shared_ptr<BLEGattService> &nusService,
                     const std::shared_ptr<BLEGattCharacteristic> &txCharacteristic,
                     const std::shared_ptr<BLEGattDescriptor> &txDescriptor,
                     const std::shared_ptr<BLEGattCharacteristic> &rxCharacteristic,
                     const std::shared_ptr<BLEGattDescriptor> &rxDescriptor);

    std::vector<uint8_t> getLastMessage();

    std::vector<uint8_t> getMessage();
    bool send(std::vector<uint8_t> payload);
    void connect();
    void disconnect();

    void notify(std::shared_ptr<std::vector<uint8_t>> value) override ;

    void notify1(std::shared_ptr<Proxy> data,
                std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>> property);


private:
    std::shared_ptr<BLEGattService> nusService;

    std::shared_ptr<BLEGattCharacteristic> txCharacteristic;
    std::shared_ptr<BLEGattDescriptor> txDescriptor;

    std::shared_ptr<BLEGattCharacteristic> rxCharacteristic;
    std::shared_ptr<BLEGattDescriptor> rxDescriptor;

    std::queue<std::vector<uint8_t >> unrequestedMessages;
};


#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_BLECONNECTION_H
