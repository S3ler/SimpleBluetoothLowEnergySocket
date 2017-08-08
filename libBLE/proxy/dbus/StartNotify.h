//
// Created by bele on 07.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_STARTNOTIFY_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_STARTNOTIFY_H


#include <enum.h>
#include <safe_flag.h>
#include <gdbus/gdbus.h>
#include <vector>

#include "parserrorname.h"


BETTER_ENUM(StartNotifyReturn, int,
            Success,
            Failed,
            NotPermitted,
            InProgress,
            NotSupported)


class StartNotifyUserData {
public:
    StartNotifyUserData();
    StartNotifyReturn getReturnValue() const;
    void setReturnValue(const StartNotifyReturn &returnValue);
    void wait();
private:
    safe_flag cmd_done;
    StartNotifyReturn returnValue;
};

extern "C"{
StartNotifyReturn startNotify(GDBusProxy* proxy);
void startNotifyReply(DBusMessage *message, void *userData);
}

#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_STARTNOTIFY_H
