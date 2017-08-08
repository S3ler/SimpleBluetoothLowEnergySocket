//
// Created by bele on 03.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_STARTDISCOVERY_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_STARTDISCOVERY_H

#include <enum.h>
#include <safe_flag.h>
#include <gdbus/gdbus.h>
#include <proxy/dbus/parserrorname.h>

/**
 * Succes - method call succeded, all other values are fail values
 * NotReady - most likely: device not power on
 * Failed - unknown error
 */
BETTER_ENUM(StartDiscoveryReturn, int,
            Success,
            NotReady,
            Failed)

class StartDiscoveryUserData {
public:
    StartDiscoveryUserData();

    StartDiscoveryReturn getReturnValue() const;

    void setReturnValue(const StartDiscoveryReturn &returnValue);

    void wait();

private:
    safe_flag cmd_done;
    StartDiscoveryReturn returnValue;
};

extern "C"{
StartDiscoveryReturn startDiscovery(GDBusProxy* proxy);
void startDiscoveryReply(DBusMessage *message, void *userData);
}

#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_STARTDISCOVERY_H
