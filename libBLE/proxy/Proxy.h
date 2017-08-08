//
// Created by bele on 01.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_PROXY_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_PROXY_H


#include <gdbus/gdbus.h>
#include <memory>
#include <list>
#include <unordered_map>
#include <mutex>
#include <cstring>
#include <helper/Observer.h>
#include <proxy/dbus/getsetdbusvalue.h>

class Observer;

class Proxy {
public:
    Proxy();

    explicit Proxy(GDBusProxy *proxy);

    virtual ~Proxy();

    virtual bool isDBusChild(GDBusProxy *proxy)=0;

    bool isDBusChild(GDBusProxy *proxy, const char *name);

    bool isChild(GDBusProxy* proxy);

    void addChild(GDBusProxy *proxy, std::shared_ptr<Proxy> child);

    std::shared_ptr<Proxy> getChild(GDBusProxy *proxy);

    std::list<std::shared_ptr<Proxy>> getChildrenAsList();

    bool removeChild(GDBusProxy *proxy);

    void setRemoved();

    bool isRemoved();

    void lockChildren();

    void unlockChildren();

protected:
    GDBusProxy *proxy;

    std::unordered_map<GDBusProxy *,std::shared_ptr<Proxy>> children;
private:
    std::mutex children_mutex;

    bool removed;

public:
    // observer pattern
public:
    void setInvalidPropertyIntegral(int invalidPropertyIntegral);

    void addObserver(std::shared_ptr<Observer> observer);

    void removeObserver(std::shared_ptr<Observer> observer);

    //bool isRegistered(std::shared_ptr<Observer> observer);

    void notifyObserver(std::shared_ptr<Proxy> object);

    void notifyObserver(std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>> property);

    void notifyObserver(std::shared_ptr<Proxy> object,
                        std::pair<int, std::pair<PROPERTY_TYPE, std::shared_ptr<void>>> property);

private:
    int invalidPropertyIntegral = -1;
    std::mutex observers_mutex;
    std::list<std::shared_ptr<Observer>> observers;
};


#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_PROXY_H
