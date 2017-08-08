//
// Created by bele on 07.08.17.
//

#include "StartNotify.h"

StartNotifyUserData::StartNotifyUserData() : returnValue(StartNotifyReturn::Failed) {
    cmd_done.reset();
}

StartNotifyReturn StartNotifyUserData::getReturnValue() const {
    return returnValue;
}

void StartNotifyUserData::setReturnValue(const StartNotifyReturn &returnValue) {
    cmd_done.set();
    StartNotifyUserData::returnValue = returnValue;
}

void StartNotifyUserData::wait() {
    cmd_done.wait();
}

StartNotifyReturn startNotify(GDBusProxy* proxy){
    const char *method = "StartNotify";
    StartNotifyUserData userData;
    if (g_dbus_proxy_method_call(proxy,
                                 method,
                                 NULL,
                                 startNotifyReply,
                                 (void *) &userData,
                                 NULL) == FALSE) {
        return userData.getReturnValue();
    }
    userData.wait();
    return userData.getReturnValue();
}

void startNotifyReply(DBusMessage *message, void *userData){
    StartNotifyUserData *returnValueHandler = reinterpret_cast<StartNotifyUserData *>(userData);
    DBusError error;
    dbus_error_init(&error);
    if (dbus_set_error_from_message(&error, message) == TRUE) {
        returnValueHandler->setReturnValue(StartNotifyReturn::_from_string(parseErrorName(error.name).c_str()));
        dbus_error_free(&error);
        return;
    }
    returnValueHandler->setReturnValue(StartNotifyReturn::Success);
}