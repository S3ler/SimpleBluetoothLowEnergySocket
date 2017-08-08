//
// Created by bele on 30.07.17.
//


#include <BLE.h>
#include "getsetdbusvalue.h"


void proxy_leak(gpointer data) {
    // TODO I do not now when this method is called: find it out
    printf("Leaking proxy %p\n", data);
}

void dbus_connect_handler(DBusConnection *connection, void *user_data) {
    BLE *ble = static_cast<BLE *>(user_data);
    ble->setDusConnected();
}

void proxy_added(GDBusProxy *proxy, void *user_data) {
    BLE *ble = static_cast<BLE *>(user_data);
    ble->addProxy(proxy);
}

void proxy_removed(GDBusProxy *proxy, void *user_data) {
    const char *interface;
    BLE *ble = static_cast<BLE *>(user_data);
    ble->removeProxy(proxy);
}

void property_changed(GDBusProxy *proxy, const char *name,
                      DBusMessageIter *iter, void *user_data) {
    if(iter == NULL){
        return;
    }
    BLE *ble = static_cast<BLE *>(user_data);
    std::string stringName(name);
    PROPERTY_TYPE propertyType = getPropertyTypeFromDBusMessageIter(iter);
    // TODO convert dbus types to known types
    // TODO converton is done in getPropertyFromIter
    switch (propertyType) {
        case PROPERTY_TYPE::BOOLEAN: {
            std::shared_ptr<dbus_bool_t> valbool (new dbus_bool_t);
            getPropertyFromIter(iter,propertyType, (void*) (valbool.get()));
            std::pair<PROPERTY_TYPE,std::shared_ptr<void>> property = std::make_pair(propertyType, valbool);
            ble->changeProperty(proxy,name ,property);
        }
            break;
        case PROPERTY_TYPE::BYTE: {
            uint8_t valbyte;
            getPropertyFromIter(iter,propertyType, (void*) &valbyte);
            //ble->changeProperty(proxy, name, (void*) &valbyte);
        }
            break;
        case PROPERTY_TYPE::UINT16: {
            dbus_uint16_t valu16;
            getPropertyFromIter(iter,propertyType, (void*) &valu16);
            //ble->changeProperty(proxy, name, (void*) &valu16);
        }
            break;
        case PROPERTY_TYPE::INT16: {
            dbus_int16_t vals16;
            getPropertyFromIter(iter,propertyType, (void*) &vals16);
            //ble->changeProperty(proxy, name, (void*) &vals16);
        }
            break;
        case PROPERTY_TYPE::UINT32: {
            dbus_uint32_t valu32;
            getPropertyFromIter(iter,propertyType, (void*) &valu32);
            //ble->changeProperty(proxy, name, (void*) &valu32);
        }
            break;
        case PROPERTY_TYPE::INT32: {
            std::shared_ptr<dbus_int32_t> vals32 (new dbus_int32_t);
            getPropertyFromIter(iter,propertyType, (void*) (vals32.get()));
            std::pair<PROPERTY_TYPE,std::shared_ptr<void>> property = std::make_pair(propertyType, vals32);
            ble->changeProperty(proxy,name ,property);
        }
            break;
        case PROPERTY_TYPE::STRING:{
            std::shared_ptr<std::string> valstring (new std::string);
            getPropertyFromIter(iter,propertyType, (void*) (valstring.get()));
            std::pair<PROPERTY_TYPE,std::shared_ptr<void>> property = std::make_pair(propertyType, valstring);
            ble->changeProperty(proxy,name ,property);
        }
            break;
        case PROPERTY_TYPE::ARRAY_OF_STRINGS: {
            std::shared_ptr<std::vector<std::string>> list (new std::vector<std::string>());
            getPropertyFromIter(iter,propertyType,(void*)(list.get()));
            std::pair<PROPERTY_TYPE,std::shared_ptr<void>> property = std::make_pair(propertyType, list);
            ble->changeProperty(proxy,name ,property);
        }
            break;
        case PROPERTY_TYPE::ARRAY_OF_BYTES:{
            std::shared_ptr<std::vector<uint8_t>> list (new std::vector<uint8_t>());
            getPropertyFromIter(iter,propertyType,(void*)(list.get()));
            std::pair<PROPERTY_TYPE,std::shared_ptr<void>> property = std::make_pair(propertyType, list);
            ble->changeProperty(proxy,name ,property);
        }
            break;
        case PROPERTY_TYPE::ARRAY_OF_OBJECTPATHES:{
            std::shared_ptr<std::vector<std::string>> list (new std::vector<std::string>());
            getPropertyFromIter(iter,propertyType,(void*)(list.get()));
            std::pair<PROPERTY_TYPE,std::shared_ptr<void>> property = std::make_pair(propertyType, list);
            ble->changeProperty(proxy,name ,property);
        }
            break;
        case PROPERTY_TYPE::DICT_OF_STRINGS: {
            // TODO
        }
            break;
        case PROPERTY_TYPE::INVALID: {
            throw std::runtime_error("property type is invalid");
        }
        default:
            // TODO
            throw std::runtime_error("property type is unknown");
            break;
    }
}

void dbus_disconnect_handler(DBusConnection *connection, void *user_data) {
    BLE *ble = static_cast<BLE *>(user_data);
    ble->setDusDisconnected();
}

void message_handler(DBusConnection *connection,
                     DBusMessage *message, void *user_data) {
    BLE *ble = reinterpret_cast<BLE *>(user_data);
    // TODO I do not now when is method is called: find it out
    printf("[SIGNAL] %s.%s\n", dbus_message_get_interface(message), dbus_message_get_member(message));
}




