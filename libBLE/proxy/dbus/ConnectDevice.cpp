//
// Created by bele on 06.08.17.
//

#include "ConnectDevice.h"

ConnectDeviceUserData::ConnectDeviceUserData() : returnValue(ConnectDeviceReturn::Failed) {
    cmd_done.reset();
}

ConnectDeviceReturn ConnectDeviceUserData::getReturnValue() const {
    return returnValue;
}

void ConnectDeviceUserData::setReturnValue(const ConnectDeviceReturn &returnValue) {
    ConnectDeviceUserData::returnValue = returnValue;
    cmd_done.set();
}

void ConnectDeviceUserData::wait() {
    cmd_done.wait();
}

ConnectDeviceReturn connectDevice(GDBusProxy* proxy){
    const char *method = "Connect";
    ConnectDeviceUserData userData;
    if (g_dbus_proxy_method_call(proxy,
                                 method,
                                 NULL,
                                 connectDeviceReply,
                                 (void *) &userData,
                                 NULL) == FALSE) {
        return userData.getReturnValue();
    }
    userData.wait();
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(5s);
    }
    return userData.getReturnValue();
}

void connectDeviceReply(DBusMessage *message, void *userData){
    ConnectDeviceUserData *returnValueHandler = reinterpret_cast<ConnectDeviceUserData *>(userData);
    DBusError error;
    dbus_error_init(&error);
    if (dbus_set_error_from_message(&error, message) == TRUE) {
        returnValueHandler->setReturnValue(ConnectDeviceReturn::_from_string(parseErrorName(error.name).c_str()));
        dbus_error_free(&error);
        return;
    }
    returnValueHandler->setReturnValue(ConnectDeviceReturn::Success);
}