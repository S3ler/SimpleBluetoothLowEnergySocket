//
// Created by bele on 30.07.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_INTERNAL_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_INTERNAL_H

#include <glib.h>
#include <dbus/dbus.h>
#include <BLE.h>
#include <printf.h>

typedef struct DBusConnection DBusConnection;
typedef struct GDBusProxy GDBusProxy;

extern "C" {

void proxy_leak(gpointer data);

void message_handler(DBusConnection *connection,
                     DBusMessage *message, void *user_data);

void dbus_connect_handler(DBusConnection *connection, void *user_data);

void dbus_disconnect_handler(DBusConnection *connection, void *user_data);

void proxy_added(GDBusProxy *proxy, void *user_data);

void proxy_removed(GDBusProxy *proxy, void *user_data);

void property_changed(GDBusProxy *proxy, const char *name,
                      DBusMessageIter *iter, void *user_data);

}
#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_INTERNAL_H
