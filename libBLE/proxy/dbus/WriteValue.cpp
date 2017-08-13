//
// Created by bele on 07.08.17.
//

#include "WriteValue.h"

WriteValueUserData::WriteValueUserData(iovec *iov) : returnValue(WriteValueReturn::Failed), iov(iov) {
    cmd_done.reset();
}

WriteValueReturn WriteValueUserData::getReturnValue() const {
    return returnValue;
}

void WriteValueUserData::setReturnValue(const WriteValueReturn &returnValue) {
    cmd_done.set();
    WriteValueUserData::returnValue= returnValue;
}

void WriteValueUserData::wait() {
    cmd_done.wait();
}

iovec *WriteValueUserData::getIov(){
    return iov;
}

WriteValueReturn writeValue(GDBusProxy *proxy, std::vector<uint8_t> value){
    // TODO memory leak (g_free!!)
    const char* method = "WriteValue";
    struct iovec iov;
    iov.iov_base = value.data();
    iov.iov_len = value.size();
    WriteValueUserData userData(&iov);
    if (g_dbus_proxy_method_call(proxy,
                                 method,
                                 writeValueSetup,
                                 writeValueReply,
                                 (void *) &userData,
                                 NULL) == FALSE) {
        return userData.getReturnValue();
    }
    userData.wait();
    return userData.getReturnValue();
}

void writeValueSetup(DBusMessageIter *iter, void *userData){
    //struct iovec *iov = user_data;
    WriteValueUserData *returnValueHandler = reinterpret_cast<WriteValueUserData *>(userData);

    struct iovec *iov=returnValueHandler->getIov();
    DBusMessageIter array, dict;


    dbus_message_iter_open_container(iter, DBUS_TYPE_ARRAY, "y", &array);
    dbus_message_iter_append_fixed_array(&array, DBUS_TYPE_BYTE,
                                         &iov->iov_base, iov->iov_len);
    dbus_message_iter_close_container(iter, &array);
    /*
    dbus_message_iter_open_container(iter, DBUS_TYPE_ARRAY,
                                     DBUS_DICT_ENTRY_BEGIN_CHAR_AS_STRING
                                             DBUS_TYPE_STRING_AS_STRING
                                             DBUS_TYPE_VARIANT_AS_STRING
                                             DBUS_DICT_ENTRY_END_CHAR_AS_STRING,
                                     &dict);

    dbus_message_iter_close_container(iter, &dict);
    */
}

void writeValueReply(DBusMessage *message, void *userData){
    WriteValueUserData *returnValueHandler = reinterpret_cast<WriteValueUserData *>(userData);
    DBusError error;
    dbus_error_init(&error);
    if (dbus_set_error_from_message(&error, message) == TRUE) {
        returnValueHandler->setReturnValue(WriteValueReturn::_from_string(parseErrorName(error.name).c_str()));
        dbus_error_free(&error);
        return;
    }
    returnValueHandler->setReturnValue(WriteValueReturn::Success);
}