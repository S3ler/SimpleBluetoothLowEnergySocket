//
// Created by bele on 01.08.17.
//

#include "CharacteristicProxy.h"


CharacteristicProxy::CharacteristicProxy(GDBusProxy *proxy) : Proxy(proxy) {
    //TODO: setInvalidPropertyIntegral(CharacteristicProperty::None);
}

bool CharacteristicProxy::isDBusChild(GDBusProxy *proxy) {
    return Proxy::isDBusChild(proxy,"Characteristic");
}

std::list<std::shared_ptr<DescriptorProxy>> CharacteristicProxy::getDescriptorProxies() {
    std::list<std::shared_ptr<DescriptorProxy>> descriptorProxiesAsList;
    for (auto &&child : children) {
        descriptorProxiesAsList.push_back(std::dynamic_pointer_cast<DescriptorProxy>(child.second));
    }
    return descriptorProxiesAsList;
}

bool CharacteristicProxy::addDescriptor(GDBusProxy *proxy) {
    if(!isDBusChild(proxy)){
        return false;
    }
    std::shared_ptr<DescriptorProxy> descriptor = std::make_shared<DescriptorProxy>(proxy);
    addChild(proxy, std::dynamic_pointer_cast<Proxy>(descriptor));
    return true;
}

std::vector<std::string> CharacteristicProxy::PropertyFlags() {
    CharacteristicProperty property = CharacteristicProperty::Flags;
    return getArrayOfStringsProperty(proxy, property._to_string());
}

std::string CharacteristicProxy::PropertyUUID() {
    CharacteristicProperty property = CharacteristicProperty::UUID;
    return getStringProperty(proxy, property._to_string());
}

StartNotifyReturn CharacteristicProxy::MethodStartNotify() {
    return startNotify(proxy);
}

StopNotifyReturn CharacteristicProxy::MethodStopNotify() {
    return stopNotify(proxy);
}

WriteValueReturn CharacteristicProxy::MethodWriteValue(std::vector<uint8_t> value) {
    return writeValue(proxy, value);
}




