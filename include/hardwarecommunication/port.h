#ifndef _OASIS_HARDWARECOMMUNICATION_PORT_H_
#define _OASIS_HARDWARECOMMUNICATION_PORT_H_

#include <stdint.h>

typedef enum port_type {
    PORT_8BIT,
    PORT_8BIT_SLOW,
    PORT_16BIT,
    PORT_32BIT
} port_type;

typedef struct port {
    port_type type;
    uint16_t port_number;
    
    union {
        void (*port_8bit_write)(uint16_t port_number, uint8_t data);
        void (*port_8bit_slow_write)(uint16_t port_number, uint8_t data); 
        void (*port_16bit_write)(uint16_t port_number, uint16_t data);
        void (*port_32bit_write)(uint16_t port_number, uint32_t data);
    } write;

    union {
        uint8_t (*port_8bit_read)(uint16_t port_number);
        uint8_t (*port_8bit_slow_read)(uint16_t port_number);
        uint16_t (*port_16bit_read)(uint16_t port_number);
        uint32_t (*port_32bit_read)(uint16_t port_number);
    } read;
} port;

struct port port_init(port_type type, uint16_t port_number);
void port_write(port *p, uint32_t data);
uint32_t port_read(port *p);

#endif // _OASIS_HARDWARECOMMUNICATION_PORT_H_
