//
// Created by bele on 05.08.17.
//

#include "BLEDevice.h"

void BLEDevice::setDeviceProxy(const std::shared_ptr<DeviceProxy> &proxy) {
    BLEDevice::deviceProxy = proxy;
}

void BLEDevice::checkDeviceProxy() const {
    if (deviceProxy == nullptr) {
        throw std::runtime_error("deviceProxy is nullptr");
    }
    if (deviceProxy->isRemoved()) {
        throw std::runtime_error("deviceProxy is removed");
    }
}

std::vector<std::string> BLEDevice::getUUIDs() const {
    return deviceProxy->PropertyUUIDs();
}

bool BLEDevice::isBlocked() const {
    return false;
}

bool BLEDevice::isConnected() const {
    return deviceProxy->PropertyConnected();
}

bool BLEDevice::isLagecyPairing() const {
    return false;
}

bool BLEDevice::isPaired() const {
    return false;
}

bool BLEDevice::isTrusted() const {
    return false;
}

int16_t BLEDevice::getRSSI() const {
    return deviceProxy->PropertyRSSI();
}

int16_t BLEDevice::getTxPower() const {
    return deviceProxy->PropertyTxPower();
}

std::string BLEDevice::getMac() const {
    return deviceProxy->PropertyAddress();
}

std::string BLEDevice::getAlias() const {
    return deviceProxy->PropertyAlias();
}

std::string BLEDevice::getIcon() const {
    return std::__cxx11::string();
}

std::string BLEDevice::getModalias() const {
    return std::__cxx11::string();
}

std::string BLEDevice::getName() const {
    return deviceProxy->PropertyName();
}

uint16_t BLEDevice::getAppearance() const {
    return 0;
}

uint32_t BLEDevice::getClass() const {
    return 0;
}

std::ostream &operator<<(std::ostream &os, const BLEDevice &device) {
    device.checkDeviceProxy();
    os << " mac: " << device.getMac();
    os << " name: " << device.getName();
    os << " alias: " << device.getAlias();
    //os << " modalias: " << adapter.getModalias();
    //os << " class: " << std::hex << adapter.getClass() << std::dec;
    os << std::boolalpha;
    os << " connected: " << device.isConnected();
    os << std::noboolalpha;
    os << " RSSI: " << device.getRSSI();
    os << " txpower: " << device.getTxPower();
    os << " UUIDs: [ ";
    std::vector<std::string> uuids = device.getUUIDs();
    std::copy(uuids.begin(), uuids.end(), infix_ostream_iterator<std::string>(os, ", "));
    os << " ] ";
    return os;
}

bool BLEDevice::hasNUS() {
    // NUS UUIDs
    std::string serviceUUID("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
    std::transform(serviceUUID.begin(), serviceUUID.end(), serviceUUID.begin(), ::tolower);

    std::string txUUID("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
    std::transform(txUUID.begin(), txUUID.end(), txUUID.begin(), ::tolower);
    std::string write("write");

    std::string rxUUID("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
    std::transform(rxUUID.begin(), rxUUID.end(), rxUUID.begin(), ::tolower);
    std::string notify("notify");

    checkDeviceProxy();
    auto bleGattServices = getGattServices();
    for (auto &&bleGattService : bleGattServices) {
        std::string uuid = bleGattService->getUUID();
        if (serviceUUID.compare(uuid) == 0) {
            auto bleGattCharacteristics = bleGattService->getCharacteristics();
            if (bleGattCharacteristics.size() == 2) {
                std::shared_ptr<BLEGattCharacteristic> txCharacteristic = nullptr;
                std::shared_ptr<BLEGattCharacteristic> rxCharacteristic = nullptr;

                for (auto &&bleGattCharacteristic : bleGattCharacteristics) {
                    if (bleGattCharacteristic->getUUID().compare(txUUID) == 0) {
                        std::vector<std::string> flags = bleGattCharacteristic->getFlags();
                        if (contains(flags, write)) {
                                txCharacteristic = bleGattCharacteristic;
                                continue;
                        }
                    }
                    if (bleGattCharacteristic->getUUID().compare(rxUUID) == 0) {
                        std::vector<std::string> flags = bleGattCharacteristic->getFlags();
                        if (contains(flags, notify)) {
                                rxCharacteristic = bleGattCharacteristic;
                                continue;
                        }
                    }
                }
                if (txCharacteristic != nullptr && rxCharacteristic != nullptr) {
                    // characteristics found
                    return true;
                }
            }
        }
    }
    return false;
}

std::shared_ptr<BLENUSConnection> BLEDevice::getNUSConnection() {
    // NUS UUIDs
    std::string serviceUUID("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
    std::transform(serviceUUID.begin(), serviceUUID.end(), serviceUUID.begin(), ::tolower);

    std::string txUUID("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
    std::transform(txUUID.begin(), txUUID.end(), txUUID.begin(), ::tolower);
    std::string write("write");

    std::string rxUUID("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
    std::transform(rxUUID.begin(), rxUUID.end(), rxUUID.begin(), ::tolower);
    std::string notify("notify");

    checkDeviceProxy();
    auto bleGattServices = getGattServices();
    for (auto &&bleGattService : bleGattServices) {
        std::string uuid = bleGattService->getUUID();
        if (serviceUUID.compare(uuid) == 0) {
            auto bleGattCharacteristics = bleGattService->getCharacteristics();
            if (bleGattCharacteristics.size() == 2) {
                std::shared_ptr<BLEGattCharacteristic> txCharacteristic = nullptr;
                std::shared_ptr<BLEGattDescriptor> txDescriptor = nullptr;

                std::shared_ptr<BLEGattCharacteristic> rxCharacteristic = nullptr;
                std::shared_ptr<BLEGattDescriptor> rxDescriptor = nullptr;

                for (auto &&bleGattCharacteristic : bleGattCharacteristics) {
                    if (bleGattCharacteristic->getUUID().compare(txUUID) == 0) {
                        //TODO check if we need all of this conditions or if write is enough
                        std::vector<std::string> flags = bleGattCharacteristic->getFlags();
                        if (contains(flags, write)) {
                            txCharacteristic = bleGattCharacteristic;
                            continue;
                        }
                    }
                    if (bleGattCharacteristic->getUUID().compare(rxUUID) == 0) {
                        std::vector<std::string> flags = bleGattCharacteristic->getFlags();
                        if (contains(flags, notify)) {
                            /*if (bleGattCharacteristic->getDescriptors().size() == 1) {
                                rxDescriptor = bleGattCharacteristic->getDescriptors().front();
                            }*/
                            rxCharacteristic = bleGattCharacteristic;
                            continue;
                        }
                    }
                }
                if (txCharacteristic != nullptr && rxCharacteristic != nullptr
                    /*&& txDescriptor != nullptr && rxDescriptor != nullptr*/) {
                    auto bleNUSConnection = std::make_shared<BLENUSConnection>(bleGattService, txCharacteristic,txDescriptor,rxCharacteristic,rxDescriptor);
                    rxCharacteristic->addValueObserver(std::static_pointer_cast<ValueObserver>(bleNUSConnection));
                    return bleNUSConnection;
                }
            }
        }
    }
    return nullptr;
}


std::list<std::shared_ptr<BLEGattService>> BLEDevice::getGattServices() {
    checkDeviceProxy();
    std::list<std::shared_ptr<BLEGattService>> bleGattServices;
    deviceProxy->lockChildren();
    for (auto &&serviceProxy : deviceProxy->getServiceProxies()) {
        std::shared_ptr<BLEGattService> bleGattService = std::make_shared<BLEGattService>();
        bleGattService->setServiceProxy(serviceProxy);
        serviceProxy->addObserver(bleGattService);
        bleGattServices.push_back(bleGattService);
    }
    deviceProxy->unlockChildren();
    return bleGattServices;
}

bool BLEDevice::connect() {
    checkDeviceProxy();

    if (!isConnected()) {
        ConnectDeviceReturn r = deviceProxy->MethodConnect();
        return r == +ConnectDeviceReturn::Success;
    }
    return true;
}

void BLEDevice::disconnect() {
    checkDeviceProxy();

    if (isConnected()) {
        DisconnectDeviceReturn r = deviceProxy->MethodDisconnect();
    }
}

void BLEDevice::notify(std::shared_ptr<Proxy> data,
                       std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>> property) {

}

bool BLEDevice::contains(std::vector<std::string> flags, std::string flag) {
    return std::find(std::begin(flags), std::end(flags), flag) != std::end(flags);
}

bool BLEDevice::isBroken() const {
    if(isConnected()){
        return false;
    }
    return getTxPower() == INT16_MAX && getRSSI() == INT16_MAX;
}

