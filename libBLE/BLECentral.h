//
// Created by bele on 28.07.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_BLECENTRAL_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_BLECENTRAL_H


#include <bits/shared_ptr.h>
#include <BLEController.h>

class BLECentral {
public:
    BLECentral(std::shared_ptr <BLEController> shared_ptr);

    std::shared_ptr<BLEConnection> awaitConnection();
};


#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_BLECENTRAL_H
