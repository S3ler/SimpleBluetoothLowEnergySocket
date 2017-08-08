//
// Created by bele on 06.08.17.
//

#include "BLENUSConnection.h"

std::vector<uint8_t> BLENUSConnection::getMessage() {
    if(unrequestedMessages.empty()){
        return std::vector<uint8_t>();
    }
    std::vector<uint8_t> msg = unrequestedMessages.front();
    unrequestedMessages.pop();
    return msg;
}

bool BLENUSConnection::send(std::vector<uint8_t> payload) {
    return txCharacteristic->writeValue(payload);
}

void BLENUSConnection::disconnect() {
    rxCharacteristic->stopNotify();
}

void BLENUSConnection::connect() {
    rxCharacteristic->startNotify();
}

BLENUSConnection::BLENUSConnection(const std::shared_ptr<BLEGattService> &nusService,
                                   const std::shared_ptr<BLEGattCharacteristic> &txCharacteristic,
                                   const std::shared_ptr<BLEGattDescriptor> &txDescriptor,
                                   const std::shared_ptr<BLEGattCharacteristic> &rxCharacteristic,
                                   const std::shared_ptr<BLEGattDescriptor> &rxDescriptor)
        : nusService(nusService), txCharacteristic(txCharacteristic), txDescriptor(txDescriptor),
          rxCharacteristic(rxCharacteristic), rxDescriptor(rxDescriptor) {}

void BLENUSConnection::notify1(std::shared_ptr<Proxy> data,
                              std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>> property) {
    if (data) {
        // TODO check if this works and is not true in any cases
        // some proxy changed
        return;
    }
    better_enums::optional<CharacteristicProperty> p = CharacteristicProperty::_from_integral_nothrow(property.first);
    if(p){
        if (*p == +CharacteristicProperty::Value) {
            std::pair<PROPERTY_TYPE, std::shared_ptr<void>> propertyValue = property.second;
            if (propertyValue.first == +PROPERTY_TYPE::ARRAY_OF_BYTES) {
                auto msg = std::static_pointer_cast<std::vector<uint8_t>>(propertyValue.second);
                unrequestedMessages.push(*msg);
            }
        }
    }

}

void BLENUSConnection::notify(std::shared_ptr<std::vector<uint8_t>> value) {
    unrequestedMessages.push(*value);
}
