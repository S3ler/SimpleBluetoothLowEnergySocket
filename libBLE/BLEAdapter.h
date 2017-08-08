//
// Created by bele on 26.07.17.
//

#ifndef BLUETOOTHCTL546_EXPERIMENTS_BLECONTROLLER_H
#define BLUETOOTHCTL546_EXPERIMENTS_BLECONTROLLER_H

#include "BLEScanner.h"
#include "safe_flag.h"

#include <future>
#include <string>
#include <mutex>
#include <set>
#include <list>
#include <helper/Observer.h>
#include <ostream>
#include <proxy/AdapterEnums.h>
#include <proxy/AdapterProxy.h>
#include "BLEScanner.h"

class BLEScanner;
class AdapterProxy;

class BLEScannnerError;

enum class BLEAdapterUsage {
    Unused,
    Central,
    Peripheral
};

class BLEAdapter : public Observer {

public:

    BLEAdapter();

    virtual ~BLEAdapter();

    void setAdapterProxy(const std::shared_ptr<AdapterProxy> &adapterProxy);

    bool startScan(BLEScanner *bleScanner);

    bool startScan();

    bool stopScan();

    void notify(std::shared_ptr<Proxy> data, std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>> property) override;

    std::list<std::shared_ptr<BLEDevice>> getDevices();

    void removeBLEScanner(BLEScanner *pScanner);

    /**
     * List of 128-bit UUIDs that represents the available local services.
     * UUIDs are encoded as string in the form 0
     *
     * Example: 00001801-0000-1000-8000-00805f9b34fb
     *
     * @note return empty string on error.
     * @return list of string of UUIDs
     */
    std::vector<std::string> getUUIDs() const;

    bool isDiscovering() const;

    bool isDiscoverable() const;

    void setDiscoverable(bool powered) const;

    bool isPairable() const;

    void setPairable(bool powered) const;

    bool isPowered() const;

    void setPowered(bool powered) const;

    /**
     * The Bluetooth device address (MAC) encoded as string in the form XX:XX:XX:XX:XX:XX.
     *
     * @note return empty string on error.
     * @return mac address of the adapter.
     */
    std::string getMac() const;

    std::string getAlias() const;

    void setAlias(std::string alias) const;

    std::string getModalias() const;

    std::string getName() const;

    uint32_t getClass() const;

    /**
     * The pairable timeout in seconds. A value of zero
     * means that the timeout is disabled and it will stay in
     * pairable mode forever.
     *
     * The default value for the discoverable timeout should be 180 seconds (3 minutes).
     *
     * @note returns UINT32_MAX on error.
     * @return DiscoverableTimeout between 0 and UINT32_MAX
     */
    uint32_t getDiscoverableTimeout() const;

    void setDiscoverableTimeout(uint32_t discoverableTimeout) const;

    /**
     * The pairable timeout in seconds. A value of zero
     * means that the timeout is disabled and it will stay in
     * pairable mode forever.
     *
     * The default value for pairable timeout should be disabled (value 0).
     *
     * @note returns UINT32_MAX on error.
     * @return PairableTImeout between 0 and UINT32_MAX
     */
    uint32_t getPairableTimeout() const;

    void setPairableTimeout(uint32_t pairableTimeout) const;

    friend std::ostream &operator<<(std::ostream &os, const BLEAdapter &adapter);

private:
    void checkAdapterProxy();

    std::shared_ptr<AdapterProxy> adapterProxy;

    void addBLEScanner(BLEScanner *pScanner);

    bool isRegistered(BLEScanner *pScanner);

    std::set<BLEScanner *> bleScanners;

    bool propertyChangeLogging = false;
public:
    void setPropertyChangeLogging(bool propertyChangeLogging);

private:
    void logNotification(std::shared_ptr<Proxy> data, std::pair<int, std::pair<PROPERTY_TYPE,std::shared_ptr<void>>> property );
};


#endif //BLUETOOTHCTL546_EXPERIMENTS_BLECONTROLLER_H
