//
// Created by bele on 06.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_DEVICEPROPERTY_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_DEVICEPROPERTY_H

#include <enum.h>

BETTER_ENUM(DeviceProperty, int,
            None=-1,
            GattServices,
            UUIDs,
            Blocked,
            Connected,
            LegacyPairing,
            Paired,
            Trusted,
            ServiceData,
            ManufacturerData,
            RSSI,
            TxPower,
            Adapter,
            Address,
            Alias,
            Modalias,
            Name,
            Appearance,
            Class)

#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_DEVICEPROPERTY_H
