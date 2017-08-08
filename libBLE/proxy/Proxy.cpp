//
// Created by bele on 01.08.17.
//

#include "Proxy.h"

Proxy::Proxy() {}

Proxy::Proxy(GDBusProxy *proxy) : proxy(proxy), removed(false) {}

Proxy::~Proxy() {

}

bool Proxy::isDBusChild(GDBusProxy *proxy, const char *name) {
    DBusMessageIter iter;
    const char *adapter, *path;
    GDBusProxy *master = this->proxy;

    if (!master)
        return true;

    if (g_dbus_proxy_get_property(proxy, name, &iter) == FALSE)
        return true;

    dbus_message_iter_get_basic(&iter, &adapter);
    path = g_dbus_proxy_get_path(master);

    if (!strcmp(path, adapter))
        return true;

    return false;
}


bool Proxy::isChild(GDBusProxy *proxy) {
    return children.find(proxy) != children.end();
}


void Proxy::addChild(GDBusProxy *proxy, std::shared_ptr<Proxy> child) {
    children.insert(std::make_pair<>(proxy, child));
    notifyObserver(child);
}


std::list<std::shared_ptr<Proxy>> Proxy::getChildrenAsList() {
    std::list<std::shared_ptr<Proxy>> childrenAsList;
    for (auto &&child : children) {
        childrenAsList.push_back(child.second);
    }
    return childrenAsList;
}


bool Proxy::removeChild(GDBusProxy *proxy) {
    if (!isChild(proxy)) {
        return false;
    }
    std::unordered_map<GDBusProxy *, std::shared_ptr<Proxy>>::const_iterator child = children.find(proxy);
    child->second->setRemoved();
    notifyObserver(child->second);
    return children.erase(proxy) > 0;
}

void Proxy::setRemoved() {
    removed = true;
    for (auto &&child : children) {
        child.second->setRemoved();
    }
    children.clear();
}


void Proxy::lockChildren() {
    children_mutex.lock();
}

void Proxy::unlockChildren() {
    children_mutex.unlock();
}

bool Proxy::isRemoved() {
    return removed;
}

// Observer pattern
void Proxy::setInvalidPropertyIntegral(int invalidPropertyIntegral) {
    Proxy::invalidPropertyIntegral = invalidPropertyIntegral;
}


void Proxy::addObserver(std::shared_ptr<Observer> observer) {
    std::lock_guard<std::mutex> observers_lock_guard(observers_mutex);
    observers.push_back(observer);
}

void Proxy::removeObserver(std::shared_ptr<Observer> observer) {
    std::lock_guard<std::mutex> observers_lock_guard(observers_mutex);
    observers.remove(observer);
}

/*bool Proxy::isRegistered(std::shared_ptr<Observer> observer) {
    std::lock_guard<std::mutex> observers_lock_guard(observers_mutex);
    return (std::find(observers.begin(), observers.end(), observer) != observers.end());
}*/

void Proxy::notifyObserver(std::shared_ptr<Proxy> object){
    std::shared_ptr<void> sharedNullptr (nullptr);
    std::pair<PROPERTY_TYPE,std::shared_ptr<void>> pairInvalid = std::make_pair(PROPERTY_TYPE::INVALID, sharedNullptr);
    std::pair<int, std::pair<PROPERTY_TYPE,std::shared_ptr<void>>> emptyProperty = std::pair<int, std::pair<PROPERTY_TYPE,std::shared_ptr<void>>>(invalidPropertyIntegral,pairInvalid);
    notifyObserver(object, emptyProperty);
}
void Proxy::notifyObserver(std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>> property) {
    notifyObserver(std::shared_ptr<Proxy>(),property);
}


void Proxy::notifyObserver(std::shared_ptr<Proxy> object, std::pair<int, std::pair<PROPERTY_TYPE,std::shared_ptr<void>>> property) {
    std::lock_guard<std::mutex> observers_lock_guard(observers_mutex);
    for (auto &&observer : observers) {
        observer->notify(object, property);
    }
}

std::shared_ptr<Proxy> Proxy::getChild(GDBusProxy *proxy) {
    auto value = children.find(proxy);
    if(value == children.end()){
        return nullptr;
    }
    return value->second;
}
