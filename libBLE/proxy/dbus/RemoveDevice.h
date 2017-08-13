//
// Created by bele on 10.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_REMOVEDEVICE_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_REMOVEDEVICE_H

#include <enum.h>
#include <safe_flag.h>
#include <gdbus/gdbus.h>
#include "parserrorname.h"

BETTER_ENUM(RemoveDeviceReturn, int,
            Success,
            InvalidArguments,
            Failed)

class RemoveDeviceUserData {
public:
    RemoveDeviceUserData(char* path);

    RemoveDeviceReturn getReturnValue() const;

    void setReturnValue(const RemoveDeviceReturn &returnValue);

    void wait();

    char *getPath();

private:
    char* path;
    safe_flag cmd_done;
    RemoveDeviceReturn returnValue;
};

extern "C"{
RemoveDeviceReturn dbusRemoveDevice(GDBusProxy *proxy, GDBusProxy *device);
void removeDeviceSetup(DBusMessageIter *iter, void *userData);
void removeDeviceReply(DBusMessage *message, void *userData);
}

#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_REMOVEDEVICE_H
