//
// Created by bele on 26.07.17.
//
#include "BLE.h"

#include <zconf.h>
#include <iostream>
#include <proxy/AdapterProxy.h>
#include <proxy/dbus/getsetdbusvalue.h>
#include <proxy/ServiceEnum.h>

void proxy_leak(gpointer data);

BLE::BLE() : BLE(nullptr) {}

BLE::BLE(GDBusProxy *proxy) : Proxy(proxy), dbusConnected(false), initialized(false) {}

bool BLE::isDBusChild(GDBusProxy *proxy) {
    return true;
}


BLE::~BLE() {
    if (!initialized) {
        return;
    }
    stop_g_main_loop();
    g_dbus_client_unref(client);
    dbus_connection_unref(dbus_conn);
    g_main_loop_unref(main_loop);

    // TODO g_list_free_full(ctrl_list, proxy_leak);

    //g_free(auto_register_agent);
}

std::list<std::shared_ptr<BLEAdapter>> BLE::getAdapters() {
    checkAndInitialize();
    std::list<std::shared_ptr<BLEAdapter>> adapters;
    lockChildren();
    for (auto &&child : children) {
        std::shared_ptr<AdapterProxy> adapterProxy = std::dynamic_pointer_cast<AdapterProxy>(child.second);
        std::shared_ptr<BLEAdapter> adapter = std::make_shared<BLEAdapter>();
        adapter->setAdapterProxy(adapterProxy);
        adapterProxy->addObserver(adapter);
        adapters.push_back(adapter);
    }
    unlockChildren();
    return adapters;
}

void BLE::checkAndInitialize() {
    if (!initialized) {
        if (!init()) {
            throw std::runtime_error("could not initialize dbus.");
        }
    }
}

bool BLE::init() {
    //auto_register_agent = g_strdup("");

    main_loop = g_main_loop_new(NULL, FALSE);
    dbus_conn = g_dbus_setup_bus(DBUS_BUS_SYSTEM, NULL, NULL);
    g_dbus_attach_object_manager(dbus_conn);


    // TODO signal = setup_signalfd();
    client = g_dbus_client_new(dbus_conn, "org.bluez", "/org/bluez");

    g_dbus_client_set_connect_watch(client, dbus_connect_handler, this); // called when dbus connection ready
    g_dbus_client_set_disconnect_watch(client, dbus_disconnect_handler, this); // called when dbus connection closed
    g_dbus_client_set_signal_watch(client, message_handler, this);
    g_dbus_client_set_proxy_handlers(client, proxy_added, proxy_removed,
                                     property_changed, this);

    //g_dbus_client_set_ready_watch(client, client_ready, this);

    dbusConnected = false;
    cmd_flag.reset();
    //adapterFound_flag.reset();

    initialized = true;
    start_g_main_loop();
    std::chrono::seconds sec(5);
    if (!cmd_flag.wait_for(sec)) {
        stop_g_main_loop();
        return false;
    }
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }

    initialized = true;
    return dbusConnected;

}

void BLE::start_g_main_loop() {
    g_main_loop_thread = std::thread(&BLE::g_main_loop_loop, this);
    g_main_loop_thread.detach();
}

void BLE::stop_g_main_loop() {
    g_main_flag.reset();
    g_main_loop_quit(main_loop);
    if (g_main_loop_thread.joinable()) {
        g_main_loop_thread.join();
    }
    g_main_flag.wait();
}

void BLE::g_main_loop_loop() {
    g_main_loop_run(main_loop);
    g_main_flag.set();
}


/*
guint BLE::setup_signalfd() {
    GIOChannel *channel;
    guint source;
    sigset_t mask;
    int fd;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);

    if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0) {
        perror("Failed to set signal mask");
        return 0;
    }

    fd = signalfd(-1, &mask, 0);
    if (fd < 0) {
        perror("Failed to create signal descriptor");
        return 0;
    }

    channel = g_io_channel_unix_new(fd);

    g_io_channel_set_close_on_unref(channel, TRUE);
    g_io_channel_set_encoding(channel, NULL, NULL);
    g_io_channel_set_buffered(channel, FALSE);

    source = g_io_add_watch(channel,
                            G_IO_IN | G_IO_HUP | G_IO_ERR | G_IO_NVAL,
                            signal_handler, NULL);

    g_io_channel_unref(channel);

    return source;
}
 */

// TODO add this method again
/*  add this method again
gboolean BLE::signal_handler(GIOChannel *channel, GIOCondition condition, gpointer user_data) {
    static bool terminated = false;
    struct signalfd_siginfo si;
    ssize_t result;
    int fd;

    if (condition & (G_IO_NVAL | G_IO_ERR | G_IO_HUP)) {
        g_main_loop_quit(main_loop);
        return FALSE;
    }

    fd = g_io_channel_unix_get_fd(channel);

    result = read(fd, &si, sizeof(si));
    if (result != sizeof(si))
        return FALSE;

    switch (si.ssi_signo) {
        case SIGINT:
            /*
             * If input was not yet setup up that means signal was received
             * while daemon was not yet running. Since user is not able
             * to terminate client by CTRL-D or typing exit treat this as
             * exit and fall through.
             */

/* fall through */
/*  add this method again
case SIGTERM:
if (!terminated) {
    g_main_loop_quit(main_loop);
}

terminated = true;
break;
}

return TRUE;
}
*/ // add this method again

void BLE::setDusConnected() {
    dbusConnected = true;
    cmd_flag.set();
}

void BLE::setDusDisconnected() {
    dbusConnected = false;
    cmd_flag.set();
    setRemoved();
    // throw std::runtime_error("dbus disconnected");
}

void BLE::addProxy(GDBusProxy *proxy) {
    std::string interface(g_dbus_proxy_get_interface(proxy));
    if (!interface.compare("org.bluez.Adapter1")) {
        addAdapter(proxy);
    } else if (!interface.compare("org.bluez.Device1")) {
        addDevice(proxy);
    } else if (!interface.compare("org.bluez.GattService1")) {
        addService(proxy);
    } else if (!interface.compare("org.bluez.GattCharacteristic1")) {
        addCharacteristic(proxy);
    } else if (!interface.compare("org.bluez.GattDescriptor1")) {
        addDescriptor(proxy);
    } else if (!interface.compare("org.bluez.AgentManager1")) {
        addAgentManager(proxy);
    } else if (!interface.compare("org.bluez.GattManager1")) {
        addGattManager(proxy);
    } else if (!interface.compare("org.bluez.LEAdvertisingManager1")) {
        addAdvertisingManager(proxy);
    }
}

void BLE::removeProxy(GDBusProxy *proxy) {
    std::string interface(g_dbus_proxy_get_interface(proxy));
    if (!interface.compare("org.bluez.Adapter1")) {
        removeAdapter(proxy);
    } else if (!interface.compare("org.bluez.Device1")) {
        removeDevice(proxy);
    } else if (!interface.compare("org.bluez.GattService1")) {
        removeService(proxy);
    } else if (!interface.compare("org.bluez.GattCharacteristic1")) {
        removeCharacteristic(proxy);
    } else if (!interface.compare("org.bluez.GattDescriptor1")) {
        removeDescriptor(proxy);
    } else if (!interface.compare("org.bluez.AgentManager1")) {
        removeAgentManager(proxy);
    } else if (!interface.compare("org.bluez.GattManager1")) {
        removeGattManager(proxy);
    } else if (!interface.compare("org.bluez.LEAdvertisingManager1")) {
        removeAdvertisingManager(proxy);
    }
}

void BLE::changeProperty(GDBusProxy *proxy, const char *name,
                         std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property) {
    std::string interface(g_dbus_proxy_get_interface(proxy));
    if (!interface.compare("org.bluez.Adapter1")) {
        changeAdapterProperty(proxy, AdapterProperty::_from_string(name), property);
    } else if (!interface.compare("org.bluez.Device1")) {
        changeDevice(proxy, DeviceProperty::_from_string(name), property);
    } else if (!interface.compare("org.bluez.GattService1")) {
        changeService(proxy, ServiceProperty::_from_string(name), property);
    } else if (!interface.compare("org.bluez.GattCharacteristic1")) {
        changeCharacteristic(proxy, CharacteristicProperty::_from_string(name), property);
    } else if (!interface.compare("org.bluez.GattDescriptor1")) {
        changeDescriptor(proxy, DescriptorProperty::_from_string(name), property);
    } else if (!interface.compare("org.bluez.AgentManager1")) {
        /* part of a later milestone */
        // changeAgentManager(proxy);
    } else if (!interface.compare("org.bluez.GattManager1")) {
        /* part of the peripheral milestone */
        // changeGattManager(proxy);
    } else if (!interface.compare("org.bluez.LEAdvertisingManager1")) {
        /* part of the peripheral milestone */
        // changeAdvertisingManager(proxy);
    }
}

void BLE::checkForNull(GDBusProxy *proxy) {
    if (proxy == nullptr) {
        throw std::runtime_error("proxy is nullptr");
    }
}

void BLE::addAdapter(GDBusProxy *proxy) {
    checkForNull(proxy);
    std::shared_ptr<AdapterProxy> adapter = std::make_shared<AdapterProxy>(proxy);
    addChild(proxy, std::dynamic_pointer_cast<Proxy>(adapter));
}

void BLE::addDevice(GDBusProxy *proxy) {
    checkForNull(proxy);
    for (auto &&adapter : children) {
        if (std::dynamic_pointer_cast<AdapterProxy>(adapter.second)->addDevice(proxy)) {
            return;
        }
    }
    throw std::runtime_error("could not find adapter");
}

void BLE::addService(GDBusProxy *proxy) {
    checkForNull(proxy);
    for (auto &&adapter : children) {
        if (std::dynamic_pointer_cast<AdapterProxy>(adapter.second)->addService(proxy)) {
            return;
        }
    }
    throw std::runtime_error("could not find device");
}

void BLE::addCharacteristic(GDBusProxy *proxy) {
    checkForNull(proxy);
    for (auto &&adapter : children) {
        if (std::dynamic_pointer_cast<AdapterProxy>(adapter.second)->addCharacteristic(proxy)) {
            return;
        }
    }
    throw std::runtime_error("could not find service");
}

void BLE::addDescriptor(GDBusProxy *proxy) {
    checkForNull(proxy);
    for (auto &&adapter : children) {
        if (std::dynamic_pointer_cast<AdapterProxy>(adapter.second)->addDescriptor(proxy)) {
            return;
        }
    }
    throw std::runtime_error("could not find characteristic");
}

void BLE::removeAdapter(GDBusProxy *proxy) {
    checkForNull(proxy);
    if (removeChild(proxy)) {
        return;
    }
    throw std::runtime_error("could not find adapter");
}

void BLE::removeDevice(GDBusProxy *proxy) {
    for (auto &&adapter : children) {
        if (std::dynamic_pointer_cast<AdapterProxy>(adapter.second)->removeDevice(proxy)) {
            return;
        }
    }
}

void BLE::removeService(GDBusProxy *proxy) {
    for (auto &&adapter : children) {
        if (std::dynamic_pointer_cast<AdapterProxy>(adapter.second)->removeService(proxy)) {
            return;
        }
    }
}

void BLE::removeCharacteristic(GDBusProxy *proxy) {
    for (auto &&adapter : children) {
        if (std::dynamic_pointer_cast<AdapterProxy>(adapter.second)->removeCharacteristic(proxy)) {
            return;
        }
    }
}

void BLE::removeDescriptor(GDBusProxy *proxy) {
    for (auto &&adapter : children) {
        if (std::dynamic_pointer_cast<AdapterProxy>(adapter.second)->removeDescriptor(proxy)) {
            return;
        }
    }
}

void BLE::changeAdapterProperty(GDBusProxy *proxy, AdapterProperty propertyName,
                                std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property) {
    for (auto &&adapter : children) {
        if (std::dynamic_pointer_cast<AdapterProxy>(adapter.second)->changeAdapterProperty(proxy,
                                                                                           propertyName,
                                                                                           property)) {
            return;
        }
        // dunno what to do
    }
    throw std::runtime_error("could not find adapter");
}

void BLE::changeDevice(GDBusProxy *proxy, DeviceProperty propertyName,
                       std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property) {
    for (auto &&adapter : children) {
        if (std::dynamic_pointer_cast<AdapterProxy>(adapter.second)->changeDeviceProperty(proxy,
                                                                                           propertyName,
                                                                                           property)) {
            return;
        }
        // dunno what to do
    }
    throw std::runtime_error("could not find adapter");
}

void BLE::changeService(GDBusProxy *proxy, ServiceProperty propertyName,
                        std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property) {
    for (auto &&adapter : children) {
        if (std::dynamic_pointer_cast<AdapterProxy>(adapter.second)->changeServiceProperty(proxy,
                                                                                          propertyName,
                                                                                          property)) {
            return;
        }
        // dunno what to do
    }
    throw std::runtime_error("could not find device");
}

void BLE::changeCharacteristic(GDBusProxy *proxy, CharacteristicProperty propertyName,
                               std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property) {
    for (auto &&adapter : children) {
        if (std::dynamic_pointer_cast<AdapterProxy>(adapter.second)->changeCharacteristicProperty(proxy,
                                                                                          propertyName,
                                                                                          property)) {
            return;
        }
        // dunno what to do
    }
    throw std::runtime_error("could not find service");
}

void BLE::changeDescriptor(GDBusProxy *proxy, DescriptorProperty propertyName,
                           std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property) {
    for (auto &&adapter : children) {
        if (std::dynamic_pointer_cast<AdapterProxy>(adapter.second)->changeDescriptorProperty(proxy,
                                                                                          propertyName,
                                                                                          property)) {
            return;
        }
        // dunno what to do
    }
    throw std::runtime_error("could not find characteristic");
}

void BLE::addAgentManager(GDBusProxy *proxy) {
    /* part of a later milestone */
}

void BLE::addGattManager(GDBusProxy *proxy) {
    /* part of the peripheral milestone */
}

void BLE::addAdvertisingManager(GDBusProxy *proxy) {
    /* part of the peripheral milestone */
}

void BLE::removeAgentManager(GDBusProxy *proxy) {
    /* part of the peripheral milestone */
}

void BLE::removeGattManager(GDBusProxy *proxy) {
    /* part of the peripheral milestone */
}

void BLE::removeAdvertisingManager(GDBusProxy *proxy) {
    /* part of the peripheral milestone */
}

void BLE::changeAgentManager(GDBusProxy *proxy, int propertyName,
                             std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property) {
    /* part of a later milestone */
}

void BLE::changeGattManager(GDBusProxy *proxy, int propertyName,
                            std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property) {
    /* part of the peripheral milestone */
}

void BLE::changeAdvertisingManager(GDBusProxy *proxy, int propertyName,
                                   std::pair<PROPERTY_TYPE, std::shared_ptr<void>> property) {
    /* part of the peripheral milestone */
}


