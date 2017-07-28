//
// Created by bele on 28.07.17.
//

#include "BLECentral.h"
#include "BLEConnection.h"

BLECentral::BLECentral(std::shared_ptr<BLEController> shared_ptr) {

}

std::shared_ptr<BLEConnection> BLECentral::awaitConnection() {
    return std::shared_ptr<BLEConnection>();
}
