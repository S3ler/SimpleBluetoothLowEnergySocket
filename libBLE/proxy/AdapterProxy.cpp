//
// Created by bele on 01.08.17.
//


#include "AdapterProxy.h"

AdapterProxy::AdapterProxy(GDBusProxy *proxy) : Proxy(proxy) {
    setInvalidPropertyIntegral(AdapterProperty::None);
}

bool AdapterProxy::isDBusChild(GDBusProxy *proxy) {
    return Proxy::isDBusChild(proxy, "Adapter");
}

bool AdapterProxy::addDevice(GDBusProxy *proxy) {
    if (!isDBusChild(proxy)) {
        return false;
    }
    std::shared_ptr<DeviceProxy> device = std::make_shared<DeviceProxy>(proxy);
    insertAllChildren(proxy, std::dynamic_pointer_cast<Proxy>(device));
    addChild(proxy, std::dynamic_pointer_cast<Proxy>(device));
    return true;
}

bool AdapterProxy::addService(GDBusProxy *proxy) {
    for (auto &&allChild : allChildren) {
        if(auto device = std::dynamic_pointer_cast<DeviceProxy>(allChild.second)){
            if(device){
                if(device->addService(proxy)){
                    insertAllChildren(proxy, device->getChild(proxy));
                    return true;
                }
            }
        }
    }
    return false;
}

bool AdapterProxy::addCharacteristic(GDBusProxy *proxy) {
    for (auto &&allChild : allChildren) {
        if(auto service = std::dynamic_pointer_cast<ServiceProxy>(allChild.second)){
            if(service){
                if(service->addCharacteristic(proxy)){
                    insertAllChildren(proxy, service->getChild(proxy));
                    return true;
                }
            }
        }
    }
    return false;
}

bool AdapterProxy::addDescriptor(GDBusProxy *proxy) {
    for (auto &&allChild : allChildren) {
        if(auto characteristic = std::dynamic_pointer_cast<CharacteristicProxy>(allChild.second)){
            if(characteristic){
                if(characteristic->addDescriptor(proxy)){
                    insertAllChildren(proxy, characteristic->getChild(proxy));
                    return true;
                }
            }
        }
    }
    return false;
}


std::list<std::shared_ptr<DeviceProxy>> AdapterProxy::getDeviceProxies() {
    std::list<std::shared_ptr<DeviceProxy>> deviceProxiesAsList;
    for (auto &&child : children) {
        deviceProxiesAsList.push_back(std::dynamic_pointer_cast<DeviceProxy>(child.second));
    }
    return deviceProxiesAsList;
}

bool AdapterProxy::removeDevice(GDBusProxy *proxy) {
    if (removeChild(proxy)) {
        eraseRemovedProxiesFromAllChildren();
    }
    return false;
}

bool AdapterProxy::removeService(GDBusProxy *proxy) {
    for (auto &&allChild : allChildren) {
        if(auto device = std::dynamic_pointer_cast<DeviceProxy>(allChild.second)){
            if(device){
                if(device->removeChild(proxy)){
                    eraseRemovedProxiesFromAllChildren();
                    return true;
                }
            }
        }
    }
    return false;
}

bool AdapterProxy::removeCharacteristic(GDBusProxy *proxy) {
    for (auto &&allChild : allChildren) {
        if(auto service = std::dynamic_pointer_cast<ServiceProxy>(allChild.second)){
            if(service){
                if(service->removeChild(proxy)){
                    eraseRemovedProxiesFromAllChildren();
                    return true;
                }
            }
        }
    }
    return false;
}

bool AdapterProxy::removeDescriptor(GDBusProxy *proxy) {
    for (auto &&allChild : allChildren) {
        if(auto characteristic = std::dynamic_pointer_cast<CharacteristicProxy>(allChild.second)){
            if(characteristic){
                if(characteristic->removeChild(proxy)){
                    eraseRemovedProxiesFromAllChildren();
                    return true;
                }
            }
        }
    }
    return false;
}

bool AdapterProxy::isAllChild(GDBusProxy *proxy) {
    return allChildren.find(proxy) != allChildren.end();
}

void AdapterProxy::eraseRemovedProxiesFromAllChildren() {
    auto iter = allChildren.begin();
    auto endIter = allChildren.end();
    for(; iter != endIter; ) {
        if (iter->second->isRemoved()) {
            allChildren.erase(iter++);
        } else {
            ++iter;
        }
    }
}

bool AdapterProxy::changeAdapterProperty(GDBusProxy *proxy, AdapterProperty propertyName,
                                         std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property) {
    if (this->proxy != proxy) {
        return false;
    }
    notifyObserver(std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>>(propertyName._to_integral(), property));
    return true;
}

bool AdapterProxy::changeDeviceProperty(GDBusProxy *proxy, DeviceProperty propertyName,
                                        std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property) {
    if(!isAllChild(proxy)){
        return false;
    }
    auto allChild = allChildren.find(proxy);
    allChild->second->notifyObserver(std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>>(propertyName._to_integral(), property));
    return true;
}

bool AdapterProxy::changeServiceProperty(GDBusProxy *proxy, ServiceProperty propertyName,
                                         std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property) {
    if(!isAllChild(proxy)){
        return false;
    }
    auto allChild = allChildren.find(proxy);
    allChild->second->notifyObserver(std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>>(propertyName._to_integral(), property));
    return true;}

bool AdapterProxy::changeCharacteristicProperty(GDBusProxy *proxy, CharacteristicProperty propertyName,
                                                std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property) {
    if(!isAllChild(proxy)){
        return false;
    }
    auto allChild = allChildren.find(proxy);
    allChild->second->notifyObserver(std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>>(propertyName._to_integral(), property));
    return true;}

bool AdapterProxy::changeDescriptorProperty(GDBusProxy *proxy, DescriptorProperty propertyName,
                                            std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property) {
    if(!isAllChild(proxy)){
        return false;
    }
    auto allChild = allChildren.find(proxy);
    allChild->second->notifyObserver(std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>>(propertyName._to_integral(), property));
    return true;}

// command functions
StartDiscoveryReturn AdapterProxy::MethodStartDiscovery() {
    return startDiscovery(proxy);
}

StopDiscoveryReturn AdapterProxy::MethodStopDiscovery() {
    // TODO implement me
    return {StopDiscoveryReturn::Failed};
}

RemoveDeviceReturn AdapterProxy::MethodRemoveDevice(std::shared_ptr<DeviceProxy> device) {
    // TODO implement me
    return {RemoveDeviceReturn::Failed};
}

SetDiscoveryFilterReturn AdapterProxy::MethodSetDiscoveryFilter() {
    // TODO implement me
    return {SetDiscoveryFilterReturn::Failed};
}

std::vector<std::string> AdapterProxy::PropertyUUIDs() {
    AdapterProperty adapterPropertyUUIDs = AdapterProperty::UUIDs;
    return getArrayOfStringsProperty(proxy, adapterPropertyUUIDs._to_string());
}

bool AdapterProxy::PropertyDiscovering() {
    return getBooleanProperty(proxy, "Discovering");
}

bool AdapterProxy::PropertyDiscoverable() {
    return getBooleanProperty(proxy, "Discoverable");
}

void AdapterProxy::PropertyDiscoverable(bool discoverable) {
    setBooleanProperty(proxy, "Discoverable", discoverable);
}

bool AdapterProxy::PropertyPairable() {
    return getBooleanProperty(proxy, "Pairable");
}

void AdapterProxy::PropertyPairable(bool pairable) {
    setBooleanProperty(proxy, "Pairable", pairable);
}

bool AdapterProxy::PropertyPowered() {
    return getBooleanProperty(proxy, "Powered");
}

void AdapterProxy::PropertyPowered(bool powered) {
    setBooleanProperty(proxy, "Powered", powered);
}

std::string AdapterProxy::PropertyAdapter() {
    return getStringProperty(proxy, "Address");
}

std::string AdapterProxy::PropertyAlias() {
    return getStringProperty(proxy, "Alias");
}

void AdapterProxy::PropertyAlias(std::string alias) {
    return setStringProperty(proxy, "Alias", alias);
}

std::string AdapterProxy::PropertyModalias() {
    return getStringProperty(proxy, "Modalias");
}

std::string AdapterProxy::PropertyName() {
    return getStringProperty(proxy, "Name");
}

uint32_t AdapterProxy::PropertyClass() {
    return getUint32Property(proxy, "Class");
}

uint32_t AdapterProxy::PropertyDiscoverableTimeout() {
    return getUint32Property(proxy, "DiscoverableTimeout");
}

void AdapterProxy::PropertyDiscoverableTimeout(uint32_t discoverableTimeout) {
    setUint32Property(proxy, "DiscoverableTimeout", discoverableTimeout);
}

uint32_t AdapterProxy::PropertyPairableTimeout() {
    return getUint32Property(proxy, "PairableTimeout");
}

void AdapterProxy::PropertyPairableTimeout(uint32_t pairableTimeout) {
    setUint32Property(proxy, "PairableTimeout", pairableTimeout);
}

void AdapterProxy::insertAllChildren(GDBusProxy *proxy, std::shared_ptr<Proxy> child) {
    allChildren.insert(std::make_pair<>(proxy,child));
}




