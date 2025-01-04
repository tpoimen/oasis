#ifndef _OASIS_DRIVERS_KEYBOARD_H_
#define _OASIS_DRIVERS_KEYBOARD_H_

#include <stdint.h>

#include <hardwarecommunication/port.h>
#include <hardwarecommunication/interrupts.h>

struct keyboard_driver
{
    port command_port;
    port data_port;
};

void keyboard_driver_init(void);
void keyboard_driver_delete(void);

uint32_t keyboard_handle_interrupt(uint32_t esp);

#endif // _OASIS_DRIVERS_KEYBOARD_H_
