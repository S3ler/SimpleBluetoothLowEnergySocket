//
// Created by bele on 06.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_BLEGATTSERVICE_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_BLEGATTSERVICE_H


#include <memory>
#include <proxy/ServiceProxy.h>
#include "BLEDevice.h"
#include "BLEGattCharacteristic.h"

class BLEGattService : public Observer{
public:
    void setServiceProxy(std::shared_ptr<ServiceProxy> serviceProxy);

    void notify(std::shared_ptr<Proxy> data, std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>> property) override;

    std::list<std::shared_ptr<BLEGattCharacteristic>> getCharacteristics();

    std::string getMac() const;

    bool isPrimary() const;

    std::string getUUID() const;

private:

    std::list<std::string> getDBusCharacteristics() const;

    std::string getDBusDevice() const;

    std::shared_ptr<ServiceProxy> serviceProxy;

    void checkServiceProxy() const;
};


#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_BLEGATTSERVICE_H
