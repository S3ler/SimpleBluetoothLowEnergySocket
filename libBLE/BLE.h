//
// Created by bele on 26.07.17.
//

#ifndef BLUETOOTHCTL546_EXPERIMENTS_BLE_H
#define BLUETOOTHCTL546_EXPERIMENTS_BLE_H


#include <memory.h>
#include <sys/signalfd.h>
#include <thread>
#include <proxy/dbus/BLEDBusConnection.h>
#include <atomic>
#include <proxy/AdapterProxy.h>
#include <proxy/Proxy.h>
#include <proxy/dbus/getsetdbusvalue.h>
#include "safe_flag.h"
#include "BLEAdapter.h"
#include <proxy/DeviceProperty.h>
#include <proxy/ServiceEnum.h>

class BLEAdapter;

class BLE : public Proxy {

public:

    BLE();

    explicit BLE(GDBusProxy *proxy);

    bool isDBusChild(GDBusProxy *proxy) override;

    virtual ~BLE();

    std::list<std::shared_ptr<BLEAdapter>> getAdapters();


    //std::shared_ptr<BLEAdapter> getDefaultAdapter();


    //std::shared_ptr<BLEAdapter> getAdapter(std::string mac);

    void checkAndInitialize();

public:

    void setDusConnected();

    void setDusDisconnected();

    void addProxy(GDBusProxy *proxy);

    void removeProxy(GDBusProxy *proxy);

    void changeProperty(GDBusProxy *proxy, const char *name, std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property);

private:
    // called by proxy_added
    void addAdapter(GDBusProxy *proxy);

    void addDevice(GDBusProxy *proxy);

    void addService(GDBusProxy *proxy);

    void addCharacteristic(GDBusProxy *proxy);

    void addDescriptor(GDBusProxy *proxy);

    void addAgentManager(GDBusProxy *proxy);

    void addGattManager(GDBusProxy *proxy);

    void addAdvertisingManager(GDBusProxy *proxy);

    // called by proxy_removed
    void removeAdapter(GDBusProxy *proxy);

    void removeDevice(GDBusProxy *proxy);

    void removeService(GDBusProxy *proxy);

    void removeCharacteristic(GDBusProxy *proxy);

    void removeDescriptor(GDBusProxy *proxy);

    void removeAgentManager(GDBusProxy *proxy);

    void removeGattManager(GDBusProxy *proxy);

    void removeAdvertisingManager(GDBusProxy *proxy);

    //called by property_changed
    void changeAdapterProperty(GDBusProxy *proxy, AdapterProperty propertyName,
                               std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property);

    void changeDevice(GDBusProxy *proxy, DeviceProperty propertyName,
                      std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property);

    void changeService(GDBusProxy *proxy, ServiceProperty propertyName,
                       std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property);

    void changeCharacteristic(GDBusProxy *proxy, CharacteristicProperty propertyName,
                              std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property);

    void changeDescriptor(GDBusProxy *proxy, DescriptorProperty propertyName,
                          std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property);

    void changeAgentManager(GDBusProxy *proxy, int propertyName,
                            std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property);

    void changeGattManager(GDBusProxy *proxy, int propertyName,
                           std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property);

    void changeAdvertisingManager(GDBusProxy *proxy, int propertyName,
                                  std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property);

private:

    void checkForNull(GDBusProxy *proxy);

    bool init();

private:
    // GDBus
    GDBusClient *client;
    DBusConnection *dbus_conn;
    GMainLoop *main_loop;

private:
    // GMainLoop handling
    void stop_g_main_loop();

    void start_g_main_loop();

    void g_main_loop_loop();

    std::thread g_main_loop_thread;
    safe_flag g_main_flag;

    // TODO guint setup_signalfd();
    // TODO gboolean signal_handler(GIOChannel *channel, GIOCondition condition, gpointer user_data);

    std::atomic_bool initialized;
    safe_flag cmd_flag;

    std::atomic_bool dbusConnected;

    //std::set<std::shared_ptr<AdapterProxy>> adapters;
};


#endif //BLUETOOTHCTL546_EXPERIMENTS_BLE_H
