//
// Created by bele on 10.08.17.
//

#include "RemoveDevice.h"

RemoveDeviceUserData::RemoveDeviceUserData(char* path) : path(path), returnValue(RemoveDeviceReturn::Failed) {
    cmd_done.reset();
}

RemoveDeviceReturn RemoveDeviceUserData::getReturnValue() const {
    return returnValue;
}

void RemoveDeviceUserData::setReturnValue(const RemoveDeviceReturn &returnValue) {
    RemoveDeviceUserData::returnValue = returnValue;
    cmd_done.set();
}

void RemoveDeviceUserData::wait() {
    cmd_done.wait();
}

char *RemoveDeviceUserData::getPath() {
    return path;
}

RemoveDeviceReturn dbusRemoveDevice(GDBusProxy *proxy, GDBusProxy *device) {
    const char *method = "RemoveDevice";
    char *path;
    path = g_strdup(g_dbus_proxy_get_path(device));
    RemoveDeviceUserData userData(path);
    if (g_dbus_proxy_method_call(proxy, method,
                                 removeDeviceSetup,
                                 removeDeviceReply,
                                 (void *) &userData,
                                 NULL) == FALSE) {
        g_free(path);
        return userData.getReturnValue();
    }
    userData.wait();
    g_free(path);
    return userData.getReturnValue();
}

void removeDeviceSetup(DBusMessageIter *iter, void *userData) {
    RemoveDeviceUserData *returnValueHandler = reinterpret_cast<RemoveDeviceUserData *>(userData);
    char* path = returnValueHandler->getPath();
    dbus_message_iter_append_basic(iter, DBUS_TYPE_OBJECT_PATH, &path);
}

void removeDeviceReply(DBusMessage *message, void *userData){
    DBusError error;
    dbus_error_init(&error);
    RemoveDeviceUserData *returnValueHandler = reinterpret_cast<RemoveDeviceUserData *>(userData);
    if (dbus_set_error_from_message(&error, message) == TRUE) {
        better_enums::optional<RemoveDeviceReturn> returnError = RemoveDeviceReturn::_from_string_nothrow(parseErrorName(error.name).c_str());
        if(returnError){
            returnValueHandler->setReturnValue(*returnError);
        }else{
            //unknown error - often unknown method but we handle it as failed
        }
        dbus_error_free(&error);
        return;
    }
    returnValueHandler->setReturnValue(RemoveDeviceReturn::Success);
}