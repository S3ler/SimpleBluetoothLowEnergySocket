//
// Created by bele on 06.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_CHARACTERISTICENUM_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_CHARACTERISTICENUM_H

#include <enum.h>

BETTER_ENUM(CharacteristicProperty, int,
            None=-1,
            UUID,
            Service,
            Value,
            WriteAcquired,
            NotifyAcquired,
            Notifying,
            Flags
            )
#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_CHARACTERISTICENUM_H
