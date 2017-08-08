//
// Created by bele on 01.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_DEVICEPROXY_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_DEVICEPROXY_H

#include "ServiceProxy.h"
#include <unordered_map>
#include "CharacteristicProxy.h"
#include <list>
#include <string>
#include <proxy/dbus/ConnectDevice.h>
#include <proxy/dbus/DisconnectDevice.h>
#include <proxy/dbus/getsetdbusvalue.h>


class CharacteristicProxy;

class DeviceProxy : public Proxy {
public:
    explicit DeviceProxy(GDBusProxy *proxy);

    virtual ~DeviceProxy();

    bool isDBusChild(GDBusProxy *proxy);

    bool addService(GDBusProxy* proxy);

    std::list<std::shared_ptr<ServiceProxy>> getServiceProxies();

public:
    // Method
    ConnectDeviceReturn MethodConnect();

    DisconnectDeviceReturn MethodDisconnect();

public:
    // Property
    std::vector<std::string> PropertyUUIDs();

    bool PropertyConnected();

    int16_t PropertyRSSI();

    int16_t PropertyTxPower();

    std::string PropertyAddress();

    std::string PropertyAlias();

    std::string PropertyName();

private:
    std::unordered_map<GDBusProxy*, std::shared_ptr<CharacteristicProxy>> characteristicProxies;


};


#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_DEVICEPROXY_H
