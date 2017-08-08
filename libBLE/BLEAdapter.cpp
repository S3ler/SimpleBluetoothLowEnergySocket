//
// Created by bele on 26.07.17.
//


#include "BLEAdapter.h"
#include "infix_iterator.h"
#include "BLEAdapterRemovedException.h"

BLEAdapter::BLEAdapter() {
}

BLEAdapter::~BLEAdapter() {

}

void BLEAdapter::notify(std::shared_ptr<Proxy> data,
                        std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>> property) {
    if (propertyChangeLogging) {
        logNotification(data, property);
    }
    if (AdapterProperty::_from_integral(property.first) != +AdapterProperty::None) {
        switch (AdapterProperty::_from_integral(property.first)) {
            case AdapterProperty::UUIDs: {
                //std::list<std::string>* uuids = (std::list<std::string>*) property.second;
            }
            case AdapterProperty::Discoverable: {
                //bool* discoverable = (bool*) property.second;
            }
            case AdapterProperty::Discovering: {
                //bool* discoverable = (bool*) property.second;
                return;
            }
            case AdapterProperty::Powered: {
                //bool* discoverable = (bool*) property.second;
                return;
            }
            default:

                break;
        }
        return;
    }
    if (std::shared_ptr<DeviceProxy> deviceProxy = std::static_pointer_cast<DeviceProxy>(data)) {
        std::shared_ptr<BLEDevice> bleDevice = std::make_shared<BLEDevice>();
              if (!bleScanners.empty()) {
            std::shared_ptr<BLEAdvertisement> bleAdvertisement = std::make_shared<BLEAdvertisement>(bleDevice);
            for (auto &&bleScanner : bleScanners) {
                bleScanner->notifyAdvertisment(bleAdvertisement);
            }
        }
    }
}

std::list<std::shared_ptr<BLEDevice>> BLEAdapter::getDevices() {
    checkAdapterProxy();
    std::list<std::shared_ptr<BLEDevice>> bleDevices;
    adapterProxy->lockChildren();
    for (auto &&child : adapterProxy->getDeviceProxies()) {
        auto bleDevice = std::make_shared<BLEDevice>();
        bleDevice->setDeviceProxy(child);
        child->addObserver(bleDevice);
        bleDevices.push_back(bleDevice);
    }
    adapterProxy->unlockChildren();
    return bleDevices;
}

bool BLEAdapter::startScan(BLEScanner *bleScanner) {
    checkAdapterProxy();
    if (isRegistered(bleScanner)) { //TODO check if isDiscovering should be also checked
        return true;
    }

    addBLEScanner(bleScanner);
    return startScan();
}

bool BLEAdapter::startScan() {
    checkAdapterProxy();
    if (!isDiscovering()) {
        if (!isPowered()) {
            setPowered(true);
        }
        StartDiscoveryReturn r = adapterProxy->MethodStartDiscovery();
        return r == +StartDiscoveryReturn::Success;
    }
    return true;
}




bool BLEAdapter::stopScan() {
    checkAdapterProxy();
    if (isDiscovering()) {
        StopDiscoveryReturn r = adapterProxy->MethodStopDiscovery();
        return r != +StopDiscoveryReturn::Success;
    }
    return true;
}

void BLEAdapter::setAdapterProxy(const std::shared_ptr<AdapterProxy> &adapterProxy) {
    this->adapterProxy = adapterProxy;
}


void BLEAdapter::checkAdapterProxy() {
    if (adapterProxy == nullptr) {
        throw std::runtime_error("adapterProxy is nullptr");
    }
    if (adapterProxy->isRemoved()) {
        throw BLEAdapterRemovedException("AdapterProxy is removed.");
    }
}

void BLEAdapter::addBLEScanner(BLEScanner *pScanner) {
    bleScanners.insert(pScanner);
}


void BLEAdapter::removeBLEScanner(BLEScanner *pScanner) {
    bleScanners.erase(pScanner);
}

bool BLEAdapter::isRegistered(BLEScanner *pScanner) {
    for (auto &&bleScanner : bleScanners) {
        if (bleScanner == pScanner) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> BLEAdapter::getUUIDs() const {
    return adapterProxy->PropertyUUIDs();
}

bool BLEAdapter::isDiscovering() const {
    return adapterProxy->PropertyDiscovering();
}

bool BLEAdapter::isDiscoverable() const {
    return adapterProxy->PropertyDiscoverable();
}

void BLEAdapter::setDiscoverable(bool discoverable) const {
    adapterProxy->PropertyDiscoverable(discoverable);
}

bool BLEAdapter::isPairable() const {
    return adapterProxy->PropertyPairable();
}

void BLEAdapter::setPairable(bool pairable) const {
    adapterProxy->PropertyPairable(pairable);
}

bool BLEAdapter::isPowered() const {
    return adapterProxy->PropertyPowered();
}

void BLEAdapter::setPowered(bool powered) const {
    adapterProxy->PropertyPowered(powered);
}

std::string BLEAdapter::getMac() const {
    return adapterProxy->PropertyAdapter();
}

std::string BLEAdapter::getAlias() const {
    return adapterProxy->PropertyAlias();
}

void BLEAdapter::setAlias(std::string alias) const {
    adapterProxy->PropertyAlias(alias);
}

std::string BLEAdapter::getModalias() const {
    return adapterProxy->PropertyModalias();
}

std::string BLEAdapter::getName() const {
    return adapterProxy->PropertyName();
}

uint32_t BLEAdapter::getClass() const {
    return adapterProxy->PropertyClass();
}

uint32_t BLEAdapter::getDiscoverableTimeout() const {
    return adapterProxy->PropertyDiscoverableTimeout();
}

void BLEAdapter::setDiscoverableTimeout(uint32_t discoverableTimeout) const {
    adapterProxy->PropertyDiscoverableTimeout(discoverableTimeout);
}

uint32_t BLEAdapter::getPairableTimeout() const {
    return adapterProxy->PropertyPairableTimeout();
}

void BLEAdapter::setPairableTimeout(uint32_t pairableTimeout) const {
    adapterProxy->PropertyPairableTimeout(pairableTimeout);
}


std::ostream &operator<<(std::ostream &os, const BLEAdapter &adapter) {
    os << " mac: " << adapter.getMac();
    os << " name: " << adapter.getName();
    os << " alias: " << adapter.getAlias();
    os << " modalias: " << adapter.getModalias();
    os << " class: " << std::hex << adapter.getClass() << std::dec;
    os << std::boolalpha;
    os << " powered: " << adapter.isPowered();
    os << " pairable: " << adapter.isPairable();
    os << " discovering: " << adapter.isDiscovering();
    os << " discoverable: " << adapter.isDiscoverable();
    os << std::noboolalpha;
    os << " discoverable timeout: " << adapter.getDiscoverableTimeout();
    os << " pairable timeout: " << adapter.getPairableTimeout();
    os << " UUIDs: [ ";
    std::vector<std::string> uuids = adapter.getUUIDs();
    std::copy(uuids.begin(), uuids.end(), infix_ostream_iterator<std::string>(os, ", "));
    os << " ] ";
    return os;
}

void BLEAdapter::setPropertyChangeLogging(bool propertyChangeLogging) {
    BLEAdapter::propertyChangeLogging = propertyChangeLogging;
}

void BLEAdapter::logNotification(std::shared_ptr<Proxy> data,
                                 std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>> property) {
    if (AdapterProperty::_from_integral(property.first) != +AdapterProperty::None) {
        switch (AdapterProperty::_from_integral(property.first)) {
            case AdapterProperty::UUIDs: {
                std::pair<PROPERTY_TYPE, std::shared_ptr<void>> propertyValue = property.second;
                if (propertyValue.first == +PROPERTY_TYPE::ARRAY_OF_STRINGS) {
                    std::shared_ptr<std::list<std::string>> uuids = std::static_pointer_cast<std::list<std::string>>(propertyValue.second);
                    std::cout << " UUIDs: [ ";
                    std::copy(uuids->begin(), uuids->end(), infix_ostream_iterator<std::string>(std::cout, ", "));
                    std::cout << " ] " << std::endl;
                }
            }
            case AdapterProperty::Discoverable: {
                std::pair<PROPERTY_TYPE, std::shared_ptr<void>> propertyValue = property.second;
                if(propertyValue.first == +PROPERTY_TYPE::BOOLEAN){
                    std::shared_ptr<dbus_bool_t> v =std::static_pointer_cast<dbus_bool_t>(propertyValue.second);
                    std::cout << "Changed Discoverable: " << std::boolalpha << *v.get() << std::noboolalpha;
                }
            }
            case AdapterProperty::Discovering: {
                std::pair<PROPERTY_TYPE, std::shared_ptr<void>> propertyValue = property.second;
                if(propertyValue.first == +PROPERTY_TYPE::BOOLEAN){
                    std::shared_ptr<dbus_bool_t> v =std::static_pointer_cast<dbus_bool_t>(propertyValue.second);
                    std::cout << "Changed Discovering: " << std::boolalpha << (bool) *v.get() << std::noboolalpha << std::endl;
                }
                return;
            }
            case AdapterProperty::Powered: {
                std::pair<PROPERTY_TYPE, std::shared_ptr<void>> propertyValue = property.second;
                if(propertyValue.first == +PROPERTY_TYPE::BOOLEAN){
                    std::shared_ptr<void> value = propertyValue.second;
                    std::shared_ptr<dbus_bool_t> v =std::static_pointer_cast<dbus_bool_t>(propertyValue.second);
                    std::cout << "Changed Powered: " << std::boolalpha << (bool) *v.get() << std::noboolalpha << std::endl;
                }
                return;
            }
            default:

                break;
        }
        return;
    }
    if (std::shared_ptr<DeviceProxy> deviceProxy = std::static_pointer_cast<DeviceProxy>(data)) {
        std::shared_ptr<BLEDevice> bleDevice = std::make_shared<BLEDevice>();
        bleDevice->setDeviceProxy(deviceProxy);
        std::cout << "New BLEDevice: " << *bleDevice << std::endl;
    }
}





