//
// Created by bele on 01.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_CHARACTERISTICPROXY_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_CHARACTERISTICPROXY_H


#include <vector>
#include <proxy/dbus/WriteValue.h>
#include <proxy/dbus/StopNotify.h>
#include <proxy/dbus/StartNotify.h>
#include "Proxy.h"
#include "DescriptorProxy.h"
#include "CharacteristicEnum.h"

class CharacteristicProxy : public Proxy {

public:
    explicit CharacteristicProxy(GDBusProxy *proxy);

    bool isDBusChild(GDBusProxy *proxy) override;

    std::list<std::shared_ptr<DescriptorProxy>> getDescriptorProxies();

    bool addDescriptor(GDBusProxy *proxy);

public:     // Methods

    StartNotifyReturn MethodStartNotify();

    StopNotifyReturn MethodStopNotify();

    WriteValueReturn MethodWriteValue(std::vector<uint8_t> value);

public:     // Properties

    std::vector<std::string> PropertyFlags();

    std::string PropertyUUID();
};


#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_CHARACTERISTICPROXY_H
