//
// Created by bele on 01.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_ADAPTERPROXY_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_ADAPTERPROXY_H

#include <gdbus/gdbus.h>
#include <unordered_map>
#include <proxy/dbus/getsetdbusvalue.h>
#include <proxy/dbus/StartDiscovery.h>
#include <cstring>
#include <memory>
#include <map>
#include <proxy/dbus/RemoveDevice.h>

#include "Proxy.h"
#include "ServiceProxy.h"
#include "DescriptorProxy.h"
#include "DeviceProxy.h"
#include "AdapterEnums.h"
#include "DeviceProperty.h"
#include "ServiceEnum.h"


class AdapterProxy : public Proxy {
public:
    explicit AdapterProxy(GDBusProxy *proxy);

    bool isDBusChild(GDBusProxy *proxy) override;

    std::list<std::shared_ptr<DeviceProxy>> getDeviceProxies();

    bool isAllChild(GDBusProxy* proxy);

    void eraseRemovedProxiesFromAllChildren();

    bool addDevice(GDBusProxy *proxy);

    bool addService(GDBusProxy *proxy);

    bool addCharacteristic(GDBusProxy *proxy);

    bool addDescriptor(GDBusProxy *proxy);

    bool removeDevice(GDBusProxy *proxy);

    bool removeService(GDBusProxy *proxy);

    bool removeCharacteristic(GDBusProxy *proxy);

    bool removeDescriptor(GDBusProxy *proxy);

    bool changeAdapterProperty(GDBusProxy *proxy, AdapterProperty propertyName,
                               std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property);
    bool changeDeviceProperty(GDBusProxy *proxy, DeviceProperty propertyName,
                               std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property);
    bool changeServiceProperty(GDBusProxy *proxy, ServiceProperty propertyName,
                               std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property);
    bool changeCharacteristicProperty(GDBusProxy *proxy, CharacteristicProperty propertyName,
                               std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property);
    bool changeDescriptorProperty(GDBusProxy *proxy, DescriptorProperty propertyName,
                               std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property);

private:
    std::unordered_map<GDBusProxy *,std::shared_ptr<Proxy>> allChildren;

public:
    // Methods
    StartDiscoveryReturn MethodStartDiscovery();

    StopDiscoveryReturn MethodStopDiscovery();

    RemoveDeviceReturn MethodRemoveDevice(std::shared_ptr<DeviceProxy> device);

    SetDiscoveryFilterReturn MethodSetDiscoveryFilter();

public:
    // Properties
    std::vector<std::string> PropertyUUIDs();

    // boolean
    bool PropertyDiscovering();

    bool PropertyDiscoverable();

    void PropertyDiscoverable(bool discoverable);

    bool PropertyPairable();

    void PropertyPairable(bool pairable);

    bool PropertyPowered();

    void PropertyPowered(bool powered);

    // string
    std::string PropertyAdapter();

    std::string PropertyAlias();

    void PropertyAlias(std::string alias);

    std::string PropertyModalias();

    std::string PropertyName();

    // uint32_t
    uint32_t PropertyClass();

    uint32_t PropertyDiscoverableTimeout();

    void PropertyDiscoverableTimeout(uint32_t discoverableTimeout);

    uint32_t PropertyPairableTimeout();

    void PropertyPairableTimeout(uint32_t pairableTimeout);

    void insertAllChildren(GDBusProxy *proxy, std::shared_ptr<Proxy> child);
};


#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_ADAPTERPROXY_H
