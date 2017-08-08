//
// Created by bele on 06.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_DISCONNECTDEVICE_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_DISCONNECTDEVICE_H


#include <enum.h>
#include <safe_flag.h>
#include <gdbus/gdbus.h>
#include <proxy/dbus/parserrorname.h>

BETTER_ENUM(DisconnectDeviceReturn, int,
            Success,
            NotConnected)


class DisconnectDeviceUserData {
public:
    DisconnectDeviceUserData();
    DisconnectDeviceReturn getReturnValue() const;
    void setReturnValue(const DisconnectDeviceReturn &returnValue);
    void wait();
private:
    safe_flag cmd_done;
    DisconnectDeviceReturn returnValue;
};


extern "C"{
DisconnectDeviceReturn disconnectDevice(GDBusProxy* proxy);
void disconnectDeviceReply(DBusMessage *message, void *userData);
}


#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_DISCONNECTDEVICE_H
