//
// Created by bele on 05.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_GETDBUSVALUE_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_GETDBUSVALUE_H

#include <glib.h>
#include <gdbus/gdbus.h>
#include <list>
#include <string>
#include <enum.h>
#include <iostream>
#include <safe_flag.h>
#include <vector>

BETTER_ENUM(SetPropertyReturn, int,
            Success,
            Failed
)

class SetPropertyUserData {
public:
    SetPropertyUserData();

    SetPropertyReturn getReturnValue() const;

    void setReturnValue(const SetPropertyReturn &returnValue);

    void wait();

private:
    SetPropertyReturn returnValue;
    safe_flag cmd_done;
};

/**
 * Object Path is seens as string
 */

BETTER_ENUM(PROPERTY_TYPE, int,
            BOOLEAN=1,
            BYTE,
            UINT16,
            INT16,
            UINT32,
            INT32,
            STRING,
            OBJECTPATH,
            ARRAY,
            ARRAY_OF_STRINGS,
            ARRAY_OF_BYTES,
            ARRAY_OF_OBJECTPATHES,
            DICT,
            DICT_OF_STRINGS,
            INVALID
)

extern "C" {

void getProperty(GDBusProxy *proxy, const char *name, PROPERTY_TYPE prop_type, void *return_value);

void setProperty(GDBusProxy *pory, const char* name, PROPERTY_TYPE prop_type, void* to_set_value);

void setPropertyReply(const DBusError *error, void *userData);

bool getBooleanProperty(GDBusProxy *proxy, const char *name);

void setBooleanProperty(GDBusProxy *proxy, const char *name, bool value);

uint32_t getUint32Property(GDBusProxy *proxy, const char *name);

uint32_t setUint32Property(GDBusProxy *proxy, const char *name, uint32_t value);

int32_t getInt32Property(GDBusProxy *proxy, const char *name);

uint16_t getUint16Property(GDBusProxy *proxy, const char *name);

int16_t getInt16Property(GDBusProxy *proxy, const char *name);

std::string getStringProperty(GDBusProxy *proxy, const char *name);

void setStringProperty(GDBusProxy *proxy, const char *name, std::string value);

std::vector<std::string> getArrayOfStringsProperty(GDBusProxy *proxy, const char *name);

PROPERTY_TYPE getPropertyTypeFromDBusMessageIter(DBusMessageIter *iter);

PROPERTY_TYPE getPropertyTypeFromDBusType(int dbus_type);

void getPropertyFromIter(DBusMessageIter *iter, PROPERTY_TYPE prop_type, void *return_value);

}
#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_GETDBUSVALUE_H
