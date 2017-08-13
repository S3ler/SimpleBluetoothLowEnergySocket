//
// Created by bele on 05.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_BLEDEVICE_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_BLEDEVICE_H

#include "BLEGattService.h"
#include "infix_iterator.h"
#include "BLENUSConnection.h"
#include "BLEAdapter.h"
#include <proxy/dbus/ConnectDevice.h>
#include <memory>
#include <proxy/DeviceProxy.h>
#include <list>
#include <string>
#include <ostream>
#include <helper/Observer.h>
#include <algorithm>

class BLEGattService;
class BLENUSConnection;

class BLEDevice : public Observer {
public:
    void setDeviceProxy(const std::shared_ptr<DeviceProxy> &proxy);

    bool hasNUS();

    std::shared_ptr<BLENUSConnection> getNUSConnection();

    void notify(std::shared_ptr<Proxy> data, std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>> property) override;

    std::list<std::shared_ptr<BLEGattService>> getGattServices();

    bool connect();

    void disconnect();

    std::vector<std::string> getUUIDs() const;

    /**
     * Checks if a device is unreachable and not in range aka broken via heuristic.
     * The bluez dbus api keeps devices in the dbus tree even when they are disconnected and not reachable.
     * But a connect to these devices always fails and takes 30 seconds and blocks when running single threaded
     * We try to detect so called broke devices by checking the following conditions:
     * device is not connected
     * device's RSSI is not accessible
     * device's TxPower is not accessible
     * If the all is true we assume the device as "broken" and we can call then remove on the corresponding adapter.
     * Notice: After calling remove the device is of course not to be used (isRemoved will return true aka throw exceptions)
     *
     * RReasons for picking these property:
     * During scanning RSSI and TxPower can be accessed.
     * While scanning is off, RSSI and TxPower is inaccessabled.
     * But if the device is already connected it is not broken.
     * Of course unconnected devices (which are able to connect) while scanning is off are detected as broken.
     * It is not perfect but better then alway trying to connect to a lot of unaccessible devices.
     */
    bool isBroken() const;

    bool isBlocked() const;

    bool isConnected() const;

    bool isLagecyPairing() const;

    bool isPaired() const;

    bool isTrusted() const;

    //std::unordered_map<std::string, ServiceData> getServicedata() const;

    //std::unordered_map<uint16_t , ManufacturerData> getManufacturerData() const;

    int16_t getRSSI() const;

    int16_t getTxPower() const;

    //BLEAdapter getAdapter();

    std::string getMac() const;

    std::string getAlias() const;

    std::string getIcon() const;

    std::string getModalias() const;

    std::string getName() const;

    uint16_t getAppearance() const;

    uint32_t getClass() const;

    friend std::ostream &operator<<(std::ostream &os, const BLEDevice &device);

private:
    void checkDeviceProxy() const;
    std::shared_ptr<DeviceProxy> deviceProxy;
public:

    bool contains(std::vector<std::string> flags, std::string flag);

private:
    friend class BLEAdapter;
};


#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_BLEDEVICE_H
