//
// Created by bele on 08.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_BLEADAPTERREMOVEDEXCEPTION_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_BLEADAPTERREMOVEDEXCEPTION_H


#include <stdexcept>

class BLEAdapterRemovedException : public std::runtime_error {
public:
    explicit BLEAdapterRemovedException(const std::string &__arg);

};


#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_BLEADAPTERREMOVEDEXCEPTION_H
