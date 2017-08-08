//
// Created by bele on 06.08.17.
//

#include "BLEGattDescriptor.h"

void BLEGattDescriptor::setDescriptorProxy(std::shared_ptr<DescriptorProxy> descriptorProxy) {
    BLEGattDescriptor::descriptorProxy = descriptorProxy;
}

void BLEGattDescriptor::checkDescriptorProxy() {
    if(descriptorProxy == nullptr){
        throw std::runtime_error("descriptorProxy is nullptr");
    }
    if(descriptorProxy->isRemoved()){
        throw std::runtime_error("descriptorProxy is removed");
    }
}

void BLEGattDescriptor::notify(std::shared_ptr<Proxy> data,
                               std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>> property) {

}
