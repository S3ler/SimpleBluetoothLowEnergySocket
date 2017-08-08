//
// Created by bele on 07.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_STOPNOTIFY_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_STOPNOTIFY_H


#include <enum.h>
#include <safe_flag.h>
#include <gdbus/gdbus.h>
#include <vector>

#include "parserrorname.h"


BETTER_ENUM(StopNotifyReturn, int,
            Success,
            Failed)

class StopNotifyUserData {
public:
    StopNotifyUserData();
    StopNotifyReturn getReturnValue() const;
    void setReturnValue(const StopNotifyReturn &returnValue);
    void wait();
private:
    safe_flag cmd_done;
    StopNotifyReturn returnValue;
};

extern "C"{
StopNotifyReturn stopNotify(GDBusProxy* proxy);
void stopNotifyReply(DBusMessage *message, void *userData);
}

#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_STOPNOTIFY_H
