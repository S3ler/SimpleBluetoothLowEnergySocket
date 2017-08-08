//
// Created by bele on 02.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_VALUEOBSERVER_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_VALUEOBSERVER_H

#include <memory>
#include <vector>

class ValueObserver {
public:
    virtual void notify(std::shared_ptr<std::vector<uint8_t>> value) = 0;
};

#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_VALUEOBSERVER_H
