//
// Created by bele on 06.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_CONNECTDEVICE_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_CONNECTDEVICE_H


#include <enum.h>
#include <safe_flag.h>
#include <gdbus/gdbus.h>
#include <proxy/dbus/parserrorname.h>
#include <thread>


BETTER_ENUM(ConnectDeviceReturn, int,
            Success,
            NotReady,
            Failed,
            InProgress,
            AlreadyConnected)


class ConnectDeviceUserData {
public:
    ConnectDeviceUserData();
    ConnectDeviceReturn getReturnValue() const;
    void setReturnValue(const ConnectDeviceReturn &returnValue);
    void wait();
private:
    safe_flag cmd_done;
    ConnectDeviceReturn returnValue;
};



extern "C"{
ConnectDeviceReturn connectDevice(GDBusProxy* proxy);
void connectDeviceReply(DBusMessage *message, void *userData);
}

#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_CONNECTDEVICE_H
