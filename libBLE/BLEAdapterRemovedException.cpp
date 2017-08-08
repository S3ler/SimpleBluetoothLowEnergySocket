//
// Created by bele on 08.08.17.
//

#include <netinet/in.h>
#include "BLEAdapterRemovedException.h"

BLEAdapterRemovedException::BLEAdapterRemovedException(const std::string &__arg) : runtime_error(__arg) {}

