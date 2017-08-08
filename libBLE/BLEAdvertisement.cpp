//
// Created by bele on 25.07.17.
//

#include "BLEAdvertisement.h"

/*
BLEAdvertisement::BLEAdvertisement(std::string mac, std::string alias, std::string name, int16_t rssi,
                                   int16_t txPower) {

}


std::string BLEAdvertisement::getMac() {
    DBusMessageIter iter;
    const char *address;
    if (g_dbus_proxy_get_property(proxy, "Address", &iter) == FALSE) {
        // TODO if we request this and the device is alreay disconnected
        // and then we throw a runtime exception
        // everything breaks very easily
        throw std::runtime_error("Could not request advertisement mac.");
    }
    dbus_message_iter_get_basic(&iter, &address);

    return std::string(address);
}

std::string BLEAdvertisement::getAlias() {

    DBusMessageIter iter;
    const char *name;

    if (g_dbus_proxy_get_property(proxy, "Alias", &iter) == TRUE) {
        dbus_message_iter_get_basic(&iter, &name);
    } else {
        name = "<unknown>";
    }

    return std::string(name);
}

std::string BLEAdvertisement::getName() {
    return std::__cxx11::string();
}

int16_t BLEAdvertisement::getRssi() {
    return 0;
}

int16_t BLEAdvertisement::getTxPower() {
    return 0;
}

// TODO get RSSI

// TODO get TxPower


*/
BLEAdvertisement::BLEAdvertisement(const std::string &mac, const std::string &name, const std::string &alias,
                                   int16_t rssi, int16_t txPower) : mac(mac), name(name), alias(alias), rssi(rssi),
                                                                    txPower(txPower) {
    auto now = std::chrono::system_clock::now();
    this->timestamp = std::chrono::system_clock::to_time_t(now);
}

BLEAdvertisement::BLEAdvertisement(std::shared_ptr<BLEDevice> bleDevice) :
        BLEAdvertisement(bleDevice->getMac(), bleDevice->getName(), bleDevice->getAlias(), bleDevice->getRSSI(),
                         bleDevice->getTxPower()) {
}


const std::string &BLEAdvertisement::getMac() const {
    return mac;
}

const std::string &BLEAdvertisement::getName() const {
    return name;
}

const std::string &BLEAdvertisement::getAlias() const {
    return alias;
}

int16_t BLEAdvertisement::getRssi() const {
    return rssi;
}

int16_t BLEAdvertisement::getTxPower() const {
    return txPower;
}

time_t BLEAdvertisement::getTimestamp() const {
    return timestamp;
}

std::ostream &operator<<(std::ostream &os, const BLEAdvertisement &advertisement) {
    os << "mac: " << advertisement.mac << " name: " << advertisement.name << " alias: " << advertisement.alias
       << " rssi: " << advertisement.rssi << " txPower: " << advertisement.txPower << " timestamp: "
       << advertisement.timestamp;
    return os;
}

