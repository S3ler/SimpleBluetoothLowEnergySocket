//
// Created by bele on 02.08.17.
//

#include "ServiceProxy.h"
#include "ServiceEnum.h"

ServiceProxy::ServiceProxy(GDBusProxy *proxy) : Proxy(proxy) {
    //TODO setInvalidPropertyIntegral(ServiceProperty::None);
}

bool ServiceProxy::isDBusChild(GDBusProxy *proxy) {
    return Proxy::isDBusChild(proxy, "Service");
}

bool ServiceProxy::addCharacteristic(GDBusProxy *proxy) {
    if (!isDBusChild(proxy)) {
        return false;
    }
    std::shared_ptr<CharacteristicProxy> characteristic = std::make_shared<CharacteristicProxy>(proxy);
    addChild(proxy, std::dynamic_pointer_cast<Proxy>(characteristic));
    return true;
}

std::list<std::shared_ptr<CharacteristicProxy>> ServiceProxy::getCharacteristicProxies() {
    std::list<std::shared_ptr<CharacteristicProxy>> characteristicProxiesAsList;
    for (auto &&child : children) {
        characteristicProxiesAsList.push_back(std::dynamic_pointer_cast<CharacteristicProxy>(child.second));
    }
    return characteristicProxiesAsList;
}

std::string ServiceProxy::PropertyUUID() {
    ServiceProperty property = ServiceProperty::UUID;
    return getStringProperty(proxy, property._to_string());
}

