//
// Created by bele on 06.08.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_SERVICEENUM_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_SERVICEENUM_H

#include <enum.h>

BETTER_ENUM(ServiceProperty, int,
            None = -1,
            Characteristics,
            Primary,
            Device,
            UUID)

#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_SERVICEENUM_H
