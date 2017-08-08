//
// Created by bele on 06.08.17.
//

#include "BLEGattService.h"

void BLEGattService::setServiceProxy(std::shared_ptr<ServiceProxy> serviceProxy) {
    BLEGattService::serviceProxy = serviceProxy;
}

std::list<std::shared_ptr<BLEGattCharacteristic>> BLEGattService::getCharacteristics() {
    checkServiceProxy();
    std::list<std::shared_ptr<BLEGattCharacteristic>> bleGattCharacteristics;
    for (auto &&characteristicProxy : serviceProxy->getCharacteristicProxies()) {
        auto bleGattCharacteristic = std::make_shared<BLEGattCharacteristic>();
        bleGattCharacteristic->setCharacteristicProxy(characteristicProxy);
        characteristicProxy->addObserver(bleGattCharacteristic);
        bleGattCharacteristics.push_back(bleGattCharacteristic);
    }
    return bleGattCharacteristics;
}

void BLEGattService::checkServiceProxy() const{
    if(serviceProxy == nullptr){
        throw std::runtime_error("serviceProxy is nullpr");
    }
    if(serviceProxy->isRemoved()){
        throw std::runtime_error("serviceProxy is removed");
    }
}

void BLEGattService::notify(std::shared_ptr<Proxy> data,
                       std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>> property) {

}

std::string BLEGattService::getUUID() const {
    checkServiceProxy();
    return serviceProxy->PropertyUUID();
}
