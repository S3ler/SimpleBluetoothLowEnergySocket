//
// Created by bele on 07.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_WRITEVALUE_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_WRITEVALUE_H

#include <enum.h>
#include <safe_flag.h>
#include <gdbus/gdbus.h>
#include <sys/uio.h>
#include <vector>

#include "parserrorname.h"


BETTER_ENUM(WriteValueReturn, int,
            Success,
            Failed,
            InProgress,
            NotPermitted,
            NotAuthorized,
            NotSupported)


class WriteValueUserData {
public:
    WriteValueUserData(iovec *iov);

    WriteValueReturn getReturnValue() const;

    void setReturnValue(const WriteValueReturn &returnValue);

    void wait();

    iovec *getIov();

private:
    safe_flag cmd_done;
    WriteValueReturn returnValue;
    struct iovec *iov;
};

extern "C" {
WriteValueReturn writeValue(GDBusProxy *proxy, std::vector<uint8_t> value);
void writeValueSetup(DBusMessageIter *iter, void *userData);
void writeValueReply(DBusMessage *message, void *userData);
}

#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_WRITEVALUE_H
