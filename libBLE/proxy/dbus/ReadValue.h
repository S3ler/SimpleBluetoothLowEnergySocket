//
// Created by bele on 22.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_READVALUE_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_READVALUE_H


#include <enum.h>
#include <safe_flag.h>
#include <gdbus/gdbus.h>
#include <vector>

#include "parserrorname.h"

BETTER_ENUM(ReadValueReturn, int,
Success,
Failed,
NotPermitted,
NotAuthorized,
NotSupported)


class ReadValueUserData {
public:
    ReadValueUserData();
    ReadValueReturn getReturnValue() const;
    void setReturnValue(const ReadValueReturn &returnValue);
    void wait();

    void setData(std::vector<uint8_t> data);
    std::vector<uint8_t> getData();
private:
    safe_flag cmd_done;
    ReadValueReturn returnValue;
    std::vector<uint8_t> data;
};

extern "C"{
std::vector<uint8_t> readValue(GDBusProxy* proxy);
void readValueSetup(DBusMessageIter *iter, void *userData);
void readValueReply(DBusMessage *message, void *userData);
}

#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_READVALUE_H
