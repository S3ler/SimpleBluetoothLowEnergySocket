//
// Created by bele on 02.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_ADAPTERENUMS_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_ADAPTERENUMS_H

#include "enum.h"

BETTER_ENUM(AdapterProperty, int,
            None=-1,
            UUIDs,
            Discoverable,
            Discovering,
            Pairable,
            Powered,
            Address,
            Alias,
            Modalias,
            Name,
            Class,
            DiscoverableTimeout,
            PairableTimeout)


BETTER_ENUM(StopDiscoveryReturn, int,
            Success,
            NotReady,
            Failed,
            NotAuthorized)


BETTER_ENUM(SetDiscoveryFilterReturn, int,
            Success,
            NotReady,
            NotSupported,
            Failed)

#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_ADAPTERENUMS_H
