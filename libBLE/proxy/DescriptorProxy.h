//
// Created by bele on 01.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_DESCRIPTORPROXY_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_DESCRIPTORPROXY_H


#include "Proxy.h"
#include "DescriptorEnum.h"

class DescriptorProxy : public Proxy {
public:
    explicit DescriptorProxy(GDBusProxy *proxy);

    bool isDBusChild(GDBusProxy *proxy) override;
};


#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_DESCRIPTORPROXY_H
