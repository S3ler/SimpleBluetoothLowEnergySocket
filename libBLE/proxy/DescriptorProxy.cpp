//
// Created by bele on 01.08.17.
//

#include "DescriptorProxy.h"

DescriptorProxy::DescriptorProxy(GDBusProxy *proxy) : Proxy(proxy) {
    //TODO: setInvalidPropertyIntegral(DescriptorProperty::None);
}

bool DescriptorProxy::isDBusChild(GDBusProxy *proxy) {
    return false;
}
