//
// Created by bele on 22.08.17.
//

#include "ReadValue.h"

ReadValueUserData::ReadValueUserData() : returnValue(ReadValueReturn::Failed) {
    cmd_done.reset();
}

ReadValueReturn ReadValueUserData::getReturnValue() const {
    return returnValue;
}

void ReadValueUserData::setReturnValue(const ReadValueReturn &returnValue) {
    cmd_done.set();
    ReadValueUserData::returnValue = returnValue;
}

void ReadValueUserData::wait() {
    cmd_done.wait();
}

void ReadValueUserData::setData(std::vector<uint8_t> data) {
    ReadValueUserData::data = data;
}
std::vector<uint8_t> ReadValueUserData::getData(){
    return data;
}

std::vector<uint8_t> readValue(GDBusProxy* proxy){
    const char *method = "ReadValue";
    ReadValueUserData userData;
    if(g_dbus_proxy_method_call(proxy,
                                method,
                                NULL,
                                readValueReply,
                                (void*)& userData,
                                NULL)==FALSE){
        return std::vector<uint8_t>();
    }
    userData.wait();
    if(userData.getReturnValue() != +ReadValueReturn::Success){
        return std::vector<uint8_t>();
    }
    return userData.getData();
}

void readValueSetup(DBusMessageIter *iter, void *userData){
    DBusMessageIter dict;

    dbus_message_iter_open_container(iter, DBUS_TYPE_ARRAY,"y", &dict);
    /* TODO: Add offset support */
    //dbus_message_iter_close_container(iter, &dict);
}

void readValueReply(DBusMessage *message, void *userData){
    ReadValueUserData *returnValueHandler = reinterpret_cast<ReadValueUserData *>(userData);

    DBusMessageIter iter, array;
    uint8_t *value;
    int len;

    DBusError error;
    dbus_error_init(&error);
    if (dbus_set_error_from_message(&error, message) == TRUE) {
        better_enums::optional<ReadValueReturn> returnValue = ReadValueReturn::_from_string_nothrow(parseErrorName(error.name).c_str());
        if(returnValue){
            returnValueHandler->setReturnValue(*returnValue);
        }else{
            returnValueHandler->setReturnValue(ReadValueReturn::Failed);
            // FAILED
        }
        dbus_error_free(&error);
        return;
    }

    dbus_message_iter_init(message, &iter);
    if (dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_ARRAY) {
        returnValueHandler->setReturnValue(ReadValueReturn::Failed);
        return;
    }

    dbus_message_iter_recurse(&iter, &array);
    dbus_message_iter_get_fixed_array(&array, &value, &len);

    if (len < 0) {
        returnValueHandler->setReturnValue(ReadValueReturn::Failed);
        return;
    }

    std::vector<uint8_t> data;
    for(size_t i = 0; i < len;i++){
        data.push_back(value[i]);
    }
    returnValueHandler->setData(data);
    returnValueHandler->setReturnValue(ReadValueReturn::Success);
}