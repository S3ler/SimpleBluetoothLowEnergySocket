//
// Created by bele on 03.08.17.
//

#include "StartDiscovery.h"

StartDiscoveryUserData::StartDiscoveryUserData() : returnValue(StartDiscoveryReturn::Failed){
    cmd_done.reset();
}

StartDiscoveryReturn StartDiscoveryUserData::getReturnValue() const {
    return returnValue;
}

void StartDiscoveryUserData::setReturnValue(const StartDiscoveryReturn &returnValue) {
    cmd_done.set();
    StartDiscoveryUserData::returnValue = returnValue;
}

void StartDiscoveryUserData::wait() {
    cmd_done.wait();
}


StartDiscoveryReturn startDiscovery(GDBusProxy* proxy){
    const char *method = "StartDiscovery";
    StartDiscoveryUserData userData;
    if (g_dbus_proxy_method_call(proxy,
                                 method,
                                 NULL,
                                 startDiscoveryReply,
                                 (void *) &userData,
                                 NULL) == FALSE) {
        return userData.getReturnValue();
    }
    userData.wait();
    return userData.getReturnValue();
}

void startDiscoveryReply(DBusMessage *message, void *userData) {
    StartDiscoveryUserData *returnValueHandler = reinterpret_cast<StartDiscoveryUserData *>(userData);
    DBusError error;
    dbus_error_init(&error);
    if (dbus_set_error_from_message(&error, message) == TRUE) {
        returnValueHandler->setReturnValue(StartDiscoveryReturn::_from_string(parseErrorName(error.name).c_str()));
        dbus_error_free(&error);
        return;
    }
    returnValueHandler->setReturnValue(StartDiscoveryReturn::Success);
}
