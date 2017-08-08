//
// Created by bele on 01.08.17.
//

#include "DeviceProxy.h"

DeviceProxy::DeviceProxy(GDBusProxy *proxy) : Proxy(proxy) {}

bool DeviceProxy::isDBusChild(GDBusProxy *proxy) {
    return Proxy::isDBusChild(proxy, "Device");
}

bool DeviceProxy::addService(GDBusProxy *proxy) {
    if(!isDBusChild(proxy)){
        return false;
    }
    std::shared_ptr<ServiceProxy> service = std::make_shared<ServiceProxy>(proxy);
    addChild(proxy, std::dynamic_pointer_cast<Proxy>(service));

    return true;
}

std::list<std::shared_ptr<ServiceProxy>> DeviceProxy::getServiceProxies() {
    std::list<std::shared_ptr<ServiceProxy>> serviceProxiesAsList;
    for (auto &&child : children) {
        serviceProxiesAsList.push_back(std::dynamic_pointer_cast<ServiceProxy>(child.second));
    }
    return serviceProxiesAsList;
}

DeviceProxy::~DeviceProxy() {

}

std::vector<std::string> DeviceProxy::PropertyUUIDs() {
    return getArrayOfStringsProperty(proxy, "UUIDs");
}

bool DeviceProxy::PropertyConnected() {
    return getBooleanProperty(proxy, "Connected");
}

int16_t DeviceProxy::PropertyRSSI() {
    return getInt16Property(proxy, "RSSI");
}

int16_t DeviceProxy::PropertyTxPower() {
    return getInt16Property(proxy, "TxPower");
}

std::string DeviceProxy::PropertyAddress() {
    return getStringProperty(proxy, "Address");
}

std::string DeviceProxy::PropertyAlias() {
    return getStringProperty(proxy, "Alias");
}

std::string DeviceProxy::PropertyName() {
    return getStringProperty(proxy, "Name");
}

ConnectDeviceReturn DeviceProxy::MethodConnect() {
    return connectDevice(proxy);
}

DisconnectDeviceReturn DeviceProxy::MethodDisconnect() {
    return disconnectDevice(proxy);
}



