//
// Created by bele on 02.08.17.
//



#include <vector>
#include "getsetdbusvalue.h"



// TODO check if the values are returned correclty (dbus_int16 => int16_t etc)

bool getBooleanProperty(GDBusProxy *proxy, const char *name) {
    dbus_bool_t valbool = 0;
    getProperty(proxy, name, PROPERTY_TYPE::BOOLEAN, &valbool);
    return valbool;
}

void setBooleanProperty(GDBusProxy *proxy, const char *name, bool value) {
    setProperty(proxy, name, PROPERTY_TYPE::BOOLEAN, (void *) &value);
}

uint32_t getUint32Property(GDBusProxy *proxy, const char *name) {
    dbus_uint32_t valu32=UINT32_MAX;
    getProperty(proxy, name, PROPERTY_TYPE::UINT32, &valu32);
    return valu32;
}

uint32_t setUint32Property(GDBusProxy *proxy, const char *name, uint32_t value){
    setProperty(proxy, name, PROPERTY_TYPE::UINT32, (void *) &value);
}

int32_t getInt32Property(GDBusProxy *proxy, const char *name) {
    dbus_int32_t valu32 = INT32_MAX;
    getProperty(proxy, name, PROPERTY_TYPE::INT32, &valu32);
    return valu32;
}

uint16_t getUint16Property(GDBusProxy *proxy, const char *name) {
    dbus_uint16_t valu16 = UINT16_MAX;
    getProperty(proxy, name, PROPERTY_TYPE::UINT16, &valu16);
    return valu16;
}

int16_t getInt16Property(GDBusProxy *proxy, const char *name) {
    dbus_int16_t vals16 = INT16_MAX;
    getProperty(proxy, name, PROPERTY_TYPE::INT16, &vals16);
    return vals16;
}

std::string getStringProperty(GDBusProxy *proxy, const char *name) {
    std::string valstring;
    getProperty(proxy, name, PROPERTY_TYPE::STRING, &valstring);
    return valstring;
}

void setStringProperty(GDBusProxy *proxy, const char *name, std::string value) {
    setProperty(proxy, name, PROPERTY_TYPE::STRING, (void *) &value);
}

std::vector<std::string> getArrayOfStringsProperty(GDBusProxy *proxy, const char *name) {
    std::vector<std::string> vallist;
    getProperty(proxy, name, PROPERTY_TYPE::ARRAY_OF_STRINGS, &vallist);
    return vallist;
}

SetPropertyUserData::SetPropertyUserData() : returnValue(SetPropertyReturn::Failed) {
    cmd_done.reset();
}

SetPropertyReturn SetPropertyUserData::getReturnValue() const {
    return returnValue;
}

void SetPropertyUserData::setReturnValue(const SetPropertyReturn &returnValue) {
    cmd_done.set();
    SetPropertyUserData::returnValue = returnValue;
}

void SetPropertyUserData::wait() {
    cmd_done.wait();
}

void setPropertyReply(const DBusError *error, void *userData) {
    SetPropertyUserData *returnValueHandler = reinterpret_cast<SetPropertyUserData *>(userData);
    if (dbus_error_is_set(error)) {
        returnValueHandler->setReturnValue(SetPropertyReturn::Failed);
        return;
    }
    returnValueHandler->setReturnValue(SetPropertyReturn::Success);
}

void getProperty(GDBusProxy *proxy, const char *name, PROPERTY_TYPE prop_type, void *return_value) {
    DBusMessageIter iter;
    if (g_dbus_proxy_get_property(proxy, name, &iter) == FALSE) {
        return;
        /*
        std::string msg("could not request property ");
        std::string value_name(name);
        throw std::runtime_error(msg + value_name);
         */
    }
    if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_INVALID) {
        std::string msg("could not request property ");
        std::string value_name(name);
        std::string additional_message(" DBUS_TYPE_INVALID");
        throw std::runtime_error(msg + value_name + additional_message);
    }
    switch (prop_type) {
        case PROPERTY_TYPE::BOOLEAN:
            if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_BOOLEAN) {
                dbus_bool_t *value = (dbus_bool_t *) return_value;
                dbus_message_iter_get_basic(&iter, value);
                return;
            }
            break;
        case PROPERTY_TYPE::UINT16:
            if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_UINT16) {
                dbus_uint16_t *value = (dbus_uint16_t *) return_value;
                dbus_message_iter_get_basic(&iter, value);
                return;
            }
            break;
        case PROPERTY_TYPE::INT16:
            if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_INT16) {
                dbus_int16_t *value = (dbus_int16_t *) return_value;
                dbus_message_iter_get_basic(&iter, value);
                return;
            }
            break;
        case PROPERTY_TYPE::UINT32:
            if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_UINT32) {
                dbus_uint32_t *value = (dbus_uint32_t *) return_value;
                dbus_message_iter_get_basic(&iter, value);
                return;
            }
            break;
        case PROPERTY_TYPE::INT32:
            if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_INT32) {
                dbus_int32_t *value = (dbus_int32_t *) return_value;
                dbus_message_iter_get_basic(&iter, value);
                return;
            }
            break;
        case PROPERTY_TYPE::STRING:
            if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_STRING) {
                std::string *value = (std::string *) return_value;
                const char *v;
                dbus_message_iter_get_basic(&iter, &v);
                value->append(v);
                return;
            }
            break;
        case PROPERTY_TYPE::ARRAY_OF_STRINGS:
            if (g_dbus_proxy_get_property(proxy, name, &iter) == TRUE) {
                std::vector<std::string> *list_value = (std::vector<std::string> *) return_value;
                DBusMessageIter value;
                dbus_message_iter_recurse(&iter, &value);
                while (dbus_message_iter_get_arg_type(&value) == DBUS_TYPE_STRING) {
                    const char *uuid;
                    dbus_message_iter_get_basic(&value, &uuid);
                    std::string string_uuid(uuid);
                    list_value->push_back(string_uuid);
                    dbus_message_iter_next(&value);
                }
            }
            break;
        case PROPERTY_TYPE::BYTE: {
            // TODO
            throw std::runtime_error("not implemented yet");
        }
            break;
        default: {
            std::string msg("could not request property ");
            std::string value_name(name);
            std::string additional_message(" unsupported type");
            throw std::runtime_error(msg + value_name + additional_message);
        }
            break;
    }
}

void setProperty(GDBusProxy *proxy, const char *name, PROPERTY_TYPE prop_type, void *to_set_value) {
    SetPropertyUserData userData;

    switch (prop_type) {
        case PROPERTY_TYPE::BOOLEAN: {
            dbus_bool_t bValue = *((bool *) to_set_value) ? TRUE : FALSE;
            if (g_dbus_proxy_set_property_basic(proxy, name, DBUS_TYPE_BOOLEAN, &bValue, setPropertyReply,
                                                (void *) &userData, NULL)
                == FALSE) {
                // return userData.getReturnValue();
            }
            userData.wait();
            // return userData.getReturnValue();
        }
            break;
        case PROPERTY_TYPE::UINT16: {
            // TODO
            throw std::runtime_error("not implemented yet set PROPERTY_TYPE::UINT16");
        }
            break;
        case PROPERTY_TYPE::INT16: {
            // TODO
            throw std::runtime_error("not implemented yet set PROPERTY_TYPE::INT16");
        }
            break;
        case PROPERTY_TYPE::UINT32: {
            // TODO
            dbus_uint32_t uint32Value = *(uint32_t*) to_set_value;
            if (g_dbus_proxy_set_property_basic(proxy, name, DBUS_TYPE_UINT32, &uint32Value, setPropertyReply,
                                                (void *) &userData, NULL)
                == FALSE) {
                // return userData.getReturnValue();
            }
            userData.wait();
            // return userData.getReturnValue();
        }
            break;
        case PROPERTY_TYPE::INT32: {
            // TODO
            throw std::runtime_error("not implemented yet set PROPERTY_TYPE::INT32");
        }
            break;
        case PROPERTY_TYPE::STRING: {
            char* sValue = g_strdup(((std::string*)to_set_value)->c_str());
            if (g_dbus_proxy_set_property_basic(proxy, name, DBUS_TYPE_STRING, &sValue, setPropertyReply,
                                                (void *) &userData, NULL)
                == FALSE) {
                // return userData.getReturnValue();
                g_free(sValue);
            }
            userData.wait();
            g_free(sValue);

            // return userData.getReturnValue();
        }
            break;
        case PROPERTY_TYPE::ARRAY_OF_STRINGS: {
            // TODO
            throw std::runtime_error("not implemented yet");
        }
            break;
        default:
            std::string msg("could not set property ");
            std::string value_name(name);
            std::string additional_message(" unsupported type");
            throw std::runtime_error(msg + value_name + additional_message);
            break;
    }
}

PROPERTY_TYPE getPropertyTypeFromDBusMessageIter(DBusMessageIter *iter) {

    if (iter == NULL) {
        throw std::runtime_error("iter is nullptr");
    }

    switch (dbus_message_iter_get_arg_type(iter)) {
        case DBUS_TYPE_INVALID: {
            return PROPERTY_TYPE::INVALID;
        }
            break;
        case DBUS_TYPE_STRING:
            return PROPERTY_TYPE::STRING;
            break;
        case DBUS_TYPE_OBJECT_PATH:
            return PROPERTY_TYPE::OBJECTPATH;
            break;
        case DBUS_TYPE_BOOLEAN:
            return PROPERTY_TYPE::BOOLEAN;
            break;
        case DBUS_TYPE_UINT32:
            return PROPERTY_TYPE::UINT32;
            break;
        case DBUS_TYPE_UINT16:
            return PROPERTY_TYPE::UINT16;
            break;
        case DBUS_TYPE_INT16:
            return PROPERTY_TYPE::INT16;
            break;
        case DBUS_TYPE_BYTE:
            return PROPERTY_TYPE::BYTE;
            break;
        case DBUS_TYPE_VARIANT:
            // TODO
            throw std::runtime_error("not implemented yet");
            break;
        case DBUS_TYPE_ARRAY: {
            DBusMessageIter subiter;
            dbus_message_iter_recurse(iter, &subiter);
            if(getPropertyTypeFromDBusMessageIter(&subiter)==+PROPERTY_TYPE::STRING){
                return PROPERTY_TYPE::ARRAY_OF_STRINGS;
            }else if(getPropertyTypeFromDBusMessageIter(&subiter)==+PROPERTY_TYPE::BYTE){
                return PROPERTY_TYPE::ARRAY_OF_BYTES;
            }else if(getPropertyTypeFromDBusMessageIter(&subiter)==+PROPERTY_TYPE::OBJECTPATH){
                return PROPERTY_TYPE::ARRAY_OF_OBJECTPATHES;
            }else{
                    throw std::runtime_error("detection of this dbus type not implemented yet");
                }
            // can be String
            // can be object path
            // TODO
            // throw std::runtime_error("not implemented yet");
        }
            break;
        case DBUS_TYPE_DICT_ENTRY: {
            DBusMessageIter subiter;
            // TODO
            throw std::runtime_error("not implemented yet");
        }
            break;
        default:
            throw std::runtime_error("could not find out property type");
            break;
    }
}

PROPERTY_TYPE getPropertyTypeFromDBusType(int dbus_type) {
    switch (dbus_type) {
        case DBUS_TYPE_INVALID: {
            return PROPERTY_TYPE::INVALID;
        }
            break;
        case DBUS_TYPE_STRING:
            return PROPERTY_TYPE::STRING;
            break;
        case DBUS_TYPE_OBJECT_PATH:
            // TODO
            throw std::runtime_error("not implemented yet");
            break;
        case DBUS_TYPE_BOOLEAN:
            return PROPERTY_TYPE::BOOLEAN;
            break;
        case DBUS_TYPE_UINT32:
            return PROPERTY_TYPE::UINT32;
            break;
        case DBUS_TYPE_UINT16:
            return PROPERTY_TYPE::UINT16;
            break;
        case DBUS_TYPE_INT16:
            return PROPERTY_TYPE::INT16;
            break;
        case DBUS_TYPE_BYTE:
            return PROPERTY_TYPE::BYTE;
            break;
        case DBUS_TYPE_VARIANT:
            // TODO
            throw std::runtime_error("not implemented yet");
            break;
        case DBUS_TYPE_ARRAY: {
            return PROPERTY_TYPE::ARRAY;
        }
            break;
        case DBUS_TYPE_DICT_ENTRY: {
            return PROPERTY_TYPE::DICT;
        }
            break;
        default:
            throw std::runtime_error("could not find out property type");
            break;
    }
}

void getPropertyFromIter(DBusMessageIter *iter, PROPERTY_TYPE prop_type, void *return_value) {
    // TODO convert dbus types to known types

    if (dbus_message_iter_get_arg_type(iter) == DBUS_TYPE_INVALID) {
        std::string msg("could not request property from iter");
        std::string additional_message(" DBUS_TYPE_INVALID");
        throw std::runtime_error(msg + additional_message);
    }
    switch (prop_type) {
        case PROPERTY_TYPE::BOOLEAN:
            if (dbus_message_iter_get_arg_type(iter) == DBUS_TYPE_BOOLEAN) {
                dbus_bool_t *value = (dbus_bool_t *) return_value;
                dbus_message_iter_get_basic(iter, value);
                return;
            }
            throw std::runtime_error("property type do not match");
            break;
        case PROPERTY_TYPE::BYTE: {
            if (dbus_message_iter_get_arg_type(iter) == DBUS_TYPE_BYTE) {
                uint8_t *value = (uint8_t *) return_value;
                dbus_message_iter_get_basic(iter, value);
                return;
            }
        }
            throw std::runtime_error("property type do not match");
            break;
        case PROPERTY_TYPE::UINT16:
            if (dbus_message_iter_get_arg_type(iter) == DBUS_TYPE_UINT16) {
                dbus_uint16_t *value = (dbus_uint16_t *) return_value;
                dbus_message_iter_get_basic(iter, value);
                return;
            }
            throw std::runtime_error("property type do not match");
            break;
        case PROPERTY_TYPE::INT16:
            if (dbus_message_iter_get_arg_type(iter) == DBUS_TYPE_INT16) {
                dbus_int16_t *value = (dbus_int16_t *) return_value;
                dbus_message_iter_get_basic(iter, value);
                return;
            }
            throw std::runtime_error("property type do not match");
            break;
        case PROPERTY_TYPE::UINT32:
            if (dbus_message_iter_get_arg_type(iter) == DBUS_TYPE_UINT32) {
                dbus_uint32_t *value = (dbus_uint32_t *) return_value;
                dbus_message_iter_get_basic(iter, value);
                return;
            }
            throw std::runtime_error("property type do not match");
            break;
        case PROPERTY_TYPE::INT32:
            if (dbus_message_iter_get_arg_type(iter) == DBUS_TYPE_INT32) {
                dbus_int32_t *value = (dbus_int32_t *) return_value;
                dbus_message_iter_get_basic(iter, value);
                return;
            }
            throw std::runtime_error("property type do not match");
            break;
        case PROPERTY_TYPE::STRING:
            if (dbus_message_iter_get_arg_type(iter) == DBUS_TYPE_STRING) {
                std::string *value = (std::string *) return_value;
                const char *v;
                dbus_message_iter_get_basic(iter, &v);
                value->append(v);
                //g_free(v);
                return;
            }
            throw std::runtime_error("property type do not match");
            break;
        case PROPERTY_TYPE::OBJECTPATH:{
            if (dbus_message_iter_get_arg_type(iter) == DBUS_TYPE_OBJECT_PATH) {
                std::string *value = (std::string *) return_value;
                const char *v;
                dbus_message_iter_get_basic(iter, &v);
                value->append(v);
                //g_free(v);
                return;
            }
            throw std::runtime_error("property type do not match");
        }
        break;
        case PROPERTY_TYPE::ARRAY_OF_STRINGS: {
            DBusMessageIter subiter;
            dbus_message_iter_recurse(iter, &subiter);
            std::vector<std::string> *value = (std::vector<std::string> *) return_value;
            while(getPropertyTypeFromDBusMessageIter(&subiter) == +PROPERTY_TYPE::STRING){
                std::string list_entry;
                getPropertyFromIter(&subiter, PROPERTY_TYPE::STRING, (void*) &list_entry );
                value->push_back(list_entry);
                dbus_message_iter_next(&subiter);
            }
            return;
        }
            throw std::runtime_error("property type do not match");
            break;
        case PROPERTY_TYPE::ARRAY_OF_BYTES: {
            DBusMessageIter subiter;
            dbus_message_iter_recurse(iter, &subiter);
            std::vector<uint8_t> *value = (std::vector<uint8_t > *) return_value;
            while(getPropertyTypeFromDBusMessageIter(&subiter) == +PROPERTY_TYPE::BYTE) {
                uint8_t list_entry;
                getPropertyFromIter(&subiter, PROPERTY_TYPE::BYTE, (void*) &list_entry );
                value->push_back(list_entry);
                dbus_message_iter_next(&subiter);
            }
            return;
        }
            throw std::runtime_error("property type do not match");
            break;
        case PROPERTY_TYPE::ARRAY_OF_OBJECTPATHES: {
            DBusMessageIter subiter;
            dbus_message_iter_recurse(iter, &subiter);
            std::vector<std::string> *value = (std::vector<std::string> *) return_value;
            while(getPropertyTypeFromDBusMessageIter(&subiter) == +PROPERTY_TYPE::OBJECTPATH){
                std::string list_entry;
                getPropertyFromIter(&subiter, PROPERTY_TYPE::OBJECTPATH, (void*) &list_entry );
                value->push_back(list_entry);
                dbus_message_iter_next(&subiter);
            }
            return;
        }
        default: {
            std::string msg("could not request property ");
            std::string additional_message(" unsupported type");
            throw std::runtime_error(msg + additional_message);
        }
            break;
    }
}
