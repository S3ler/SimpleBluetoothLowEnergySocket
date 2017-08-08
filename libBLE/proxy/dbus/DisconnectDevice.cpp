//
// Created by bele on 06.08.17.
//

#include "DisconnectDevice.h"

DisconnectDeviceUserData::DisconnectDeviceUserData() : returnValue(DisconnectDeviceReturn::NotConnected) {
    cmd_done.reset();
}

DisconnectDeviceReturn DisconnectDeviceUserData::getReturnValue() const {
    return returnValue;
}

void DisconnectDeviceUserData::setReturnValue(const DisconnectDeviceReturn &returnValue) {
    cmd_done.set();
    DisconnectDeviceUserData::returnValue = returnValue;
}

void DisconnectDeviceUserData::wait() {
    cmd_done.wait();
}

DisconnectDeviceReturn disconnectDevice(GDBusProxy* proxy){
    const char *method = "Disconnect";
    DisconnectDeviceUserData userData;
    if (g_dbus_proxy_method_call(proxy,
                                 method,
                                 NULL,
                                 disconnectDeviceReply,
                                 (void *) &userData,
                                 NULL) == FALSE) {
        return userData.getReturnValue();
    }
    userData.wait();
    return userData.getReturnValue();
}

void disconnectDeviceReply(DBusMessage *message, void *userData){
    DisconnectDeviceUserData *returnValueHandler = reinterpret_cast<DisconnectDeviceUserData *>(userData);
    DBusError error;
    dbus_error_init(&error);
    if (dbus_set_error_from_message(&error, message) == TRUE) {
        returnValueHandler->setReturnValue(DisconnectDeviceReturn::_from_string(parseErrorName(error.name).c_str()));
        dbus_error_free(&error);
        return;
    }
    returnValueHandler->setReturnValue(DisconnectDeviceReturn::Success);
}