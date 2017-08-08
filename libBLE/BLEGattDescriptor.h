//
// Created by bele on 06.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_BLEGATTDESCRIPTOR_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_BLEGATTDESCRIPTOR_H


#include <memory>
#include <proxy/DescriptorProxy.h>
#include <vector>

class BLEGattDescriptor : public Observer{
public:
    void setDescriptorProxy(std::shared_ptr<DescriptorProxy> descriptorProxy);

    void notify(std::shared_ptr<Proxy> data, std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>> property) override;

    std::vector<uint8_t> readValue() const;

    void writeValu(std::vector<uint8_t> value);

    std::vector<uint8_t > getValue() const;

    std::string getUUID() const;

private:
    std::string getDBusCharacteristic();

    void checkDescriptorProxy();

    std::shared_ptr<DescriptorProxy> descriptorProxy;
};


#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_BLEGATTDESCRIPTOR_H
