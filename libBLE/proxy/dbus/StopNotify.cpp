//
// Created by bele on 07.08.17.
//

#include "StopNotify.h"

StopNotifyUserData::StopNotifyUserData()  :returnValue(StopNotifyReturn::Failed){
    cmd_done.reset();
}

StopNotifyReturn StopNotifyUserData::getReturnValue() const {
    return returnValue;
}

void StopNotifyUserData::setReturnValue(const StopNotifyReturn &returnValue) {
    cmd_done.set();
    StopNotifyUserData::returnValue = returnValue;
}

void StopNotifyUserData::wait() {
    cmd_done.wait();
}

StopNotifyReturn stopNotify(GDBusProxy* proxy){
    const char *method = "StopNotify";
    StopNotifyUserData userData;
    if (g_dbus_proxy_method_call(proxy,
                                 method,
                                 NULL,
                                 stopNotifyReply,
                                 (void *) &userData,
                                 NULL) == FALSE) {
        return userData.getReturnValue();
    }
    userData.wait();
    return userData.getReturnValue();
}

void stopNotifyReply(DBusMessage *message, void *userData){
    StopNotifyUserData *returnValueHandler = reinterpret_cast<StopNotifyUserData *>(userData);
    DBusError error;
    dbus_error_init(&error);
    if (dbus_set_error_from_message(&error, message) == TRUE) {
        better_enums::optional<StopNotifyReturn> returnError = StopNotifyReturn::_from_string_nothrow(parseErrorName(error.name).c_str());
        if(returnError){
            returnValueHandler->setReturnValue(*returnError);
        }else{
            //unknown error - often unknown method but we handle it as failed
        }
        dbus_error_free(&error);
        return;
    }
    returnValueHandler->setReturnValue(StopNotifyReturn::Success);
}