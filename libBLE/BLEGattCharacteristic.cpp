//
// Created by bele on 06.08.17.
//

#include "BLEGattCharacteristic.h"

void BLEGattCharacteristic::setCharacteristicProxy(std::shared_ptr<CharacteristicProxy> characteristicProxy) {
    BLEGattCharacteristic::characteristicProxy = characteristicProxy;
}

void BLEGattCharacteristic::checkCharacteristicProxy() const{
    if(characteristicProxy == nullptr){
        throw std::runtime_error("characteristicProxy is nullptr");
    }
    if(characteristicProxy->isRemoved()){
        throw std::runtime_error("characteristicProxy is removed");
    }
}

void BLEGattCharacteristic::notify(std::shared_ptr<Proxy> data,
                                   std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>> property) {

    if(!valueObservers.empty()){
        better_enums::optional<CharacteristicProperty> p = CharacteristicProperty::_from_integral_nothrow(property.first);
        if(p){
            if (*p == +CharacteristicProperty::Value) {
                std::pair<PROPERTY_TYPE, std::shared_ptr<void>> propertyValue = property.second;
                if (propertyValue.first == +PROPERTY_TYPE::ARRAY_OF_BYTES) {
                    auto value = std::static_pointer_cast<std::vector<uint8_t>>(propertyValue.second);
                    for (auto &&valueObserver : valueObservers) {
                        valueObserver->notify(value);
                    }
                }
            }
        }
    }
}

std::list<std::shared_ptr<BLEGattDescriptor>> BLEGattCharacteristic::getDescriptors() {
    checkCharacteristicProxy();
    std::list<std::shared_ptr<BLEGattDescriptor>> bleGattDescriptors;
    for (auto &&descriptorProxy : characteristicProxy->getDescriptorProxies()) {
        auto bleGattDescriptor = std::make_shared<BLEGattDescriptor>();
        bleGattDescriptor->setDescriptorProxy(descriptorProxy);
        descriptorProxy->addObserver(bleGattDescriptor);
        bleGattDescriptors.push_back(bleGattDescriptor);
    }
    return bleGattDescriptors;
}

bool BLEGattCharacteristic::startNotify() const {
    checkCharacteristicProxy();
    StartNotifyReturn r = characteristicProxy->MethodStartNotify();
    return r == +StartNotifyReturn::Success;
}

bool BLEGattCharacteristic::stopNotify() const {
    checkCharacteristicProxy();
    StopNotifyReturn r = characteristicProxy->MethodStopNotify();
    return r == +StopNotifyReturn::Success;
}

bool BLEGattCharacteristic::writeValue(std::vector<uint8_t> value) const {
    checkCharacteristicProxy();
    WriteValueReturn r = characteristicProxy->MethodWriteValue(value);
    return r == +WriteValueReturn::Success;
}

std::vector<std::string> BLEGattCharacteristic::getFlags() const{
    checkCharacteristicProxy();
    return characteristicProxy->PropertyFlags();
}

std::string BLEGattCharacteristic::getUUID() const{
    checkCharacteristicProxy();
    return characteristicProxy->PropertyUUID();
}

void BLEGattCharacteristic::addValueObserver(std::shared_ptr<ValueObserver> observer) {
    valueObservers.push_back(observer);
}




