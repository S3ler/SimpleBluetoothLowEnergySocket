//
// Created by bele on 02.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_OBSERVER_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_OBSERVER_H

#include <memory>
#include <proxy/Proxy.h>
#include <proxy/dbus/getsetdbusvalue.h>

class Proxy;

class Observer {
public:
    virtual void notify(std::shared_ptr<Proxy> data, std::pair<int, std::pair<PROPERTY_TYPE,std::shared_ptr<void>>> property ) = 0;
};

#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_OBSERVER_H
