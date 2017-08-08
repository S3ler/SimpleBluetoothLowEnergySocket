//
// Created by bele on 06.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_BLEGATTCHARACTERISTIC_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_BLEGATTCHARACTERISTIC_H

#include "BLEGattDescriptor.h"

#include <proxy/CharacteristicProxy.h>
#include <vector>
#include <helper/ValueObserver.h>

class BLEGattCharacteristic : public Observer{
public:
    void setCharacteristicProxy(std::shared_ptr<CharacteristicProxy> characteristicProxy);

    void notify(std::shared_ptr<Proxy> data, std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>> property) override;

    std::list<std::shared_ptr<BLEGattDescriptor>> getDescriptors();

    std::vector<uint8_t> readValue() const;

    bool startNotify() const;

    bool stopNotify() const;

    bool writeValue(std::vector<uint8_t> value) const;

    std::vector<uint8_t> getValue() const;

    std::vector<std::string> getFlags() const;

    bool isNotifying() const;

    std::string getUUID() const;

private:
    std::list<std::string> getDBusDescriptors() const;

    std::string getDBusService() const;

    void checkCharacteristicProxy() const;

    std::shared_ptr<CharacteristicProxy> characteristicProxy;

private:
    std::list<std::shared_ptr<ValueObserver>> valueObservers;
public:
    void addValueObserver(std::shared_ptr<ValueObserver> observer);
};


#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_BLEGATTCHARACTERISTIC_H
