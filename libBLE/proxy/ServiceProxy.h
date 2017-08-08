//
// Created by bele on 02.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_SERVICEPROXY_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_SERVICEPROXY_H


#include "Proxy.h"
#include "CharacteristicProxy.h"

class ServiceProxy : public Proxy{
public:
    explicit ServiceProxy(GDBusProxy *proxy);

    bool isDBusChild(GDBusProxy *proxy) override;

    std::list<std::shared_ptr<CharacteristicProxy>> getCharacteristicProxies();

    bool addCharacteristic(GDBusProxy* proxy);

    std::string PropertyUUID();
};


#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_SERVICEPROXY_H
