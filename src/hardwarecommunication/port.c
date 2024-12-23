#include <hardwarecommunication/port.h>

void port_8bit_write(uint16_t port_number, uint8_t data);
void port_8bit_slow_write(uint16_t port_number, uint8_t data);
void port_16bit_write(uint16_t port_number, uint16_t data);
void port_32bit_write(uint16_t port_number, uint32_t data);

uint8_t port_8bit_read(uint16_t port_number);
uint16_t port_16bit_read(uint16_t port_number);
uint32_t port_32bit_read(uint16_t port_number);

port port_init(port_type type, uint16_t port_number)
{
    port p;
    p.type = type;
    p.port_number = port_number;

    switch (type)
    {
        case PORT_8BIT:
            p.write.port_8bit_write = port_8bit_write;
            p.read.port_8bit_read = port_8bit_read;
            break;
        case PORT_8BIT_SLOW:
            p.write.port_8bit_slow_write = port_8bit_slow_write;
            p.read.port_8bit_slow_read = port_8bit_read;
            break;
        case PORT_16BIT:
            p.write.port_16bit_write = port_16bit_write;
            p.read.port_16bit_read = port_16bit_read;
            break;
        case PORT_32BIT:
            p.write.port_32bit_write = port_32bit_write;
            p.read.port_32bit_read = port_32bit_read;
            break;
    }

    return p;
}

void port_write(port *p, uint32_t data)
{
    switch (p->type)
    {
        case PORT_8BIT:
            uint8_t data_8bit = (uint8_t)data;
            p->write.port_8bit_write(p->port_number, data_8bit);
            break;
        case PORT_8BIT_SLOW:
            uint8_t data_8bit_slow = (uint8_t)data;
            p->write.port_8bit_slow_write(p->port_number, data_8bit_slow);
            break;
        case PORT_16BIT:
            uint16_t data_16bit = (uint16_t)data;
            p->write.port_16bit_write(p->port_number, data_16bit);
            break;
        case PORT_32BIT:
            p->write.port_32bit_write(p->port_number, data);
            break;
    }
}

uint32_t port_read(port *p)
{
    switch (p->type)
    {
        case PORT_8BIT:
            return (uint32_t)p->read.port_8bit_read(p->port_number);
        case PORT_8BIT_SLOW:
            return (uint32_t)p->read.port_8bit_slow_read(p->port_number);
        case PORT_16BIT:
            return (uint32_t)p->read.port_16bit_read(p->port_number);
        case PORT_32BIT:
            return p->read.port_32bit_read(p->port_number);
    }
    return 0;
}

void port_8bit_write(uint16_t port_number, uint8_t data)
{
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port_number));
}

uint8_t port_8bit_read(uint16_t port_number)
{
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port_number));
    return result;
}

void port_8bit_slow_write(uint16_t port_number, uint8_t data)
{
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (port_number));
}

void port_16bit_write(uint16_t port_number, uint16_t data)
{
    __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (port_number));
}

uint16_t port_16bit_read(uint16_t port_number)
{
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (port_number));
    return result;
}

void port_32bit_write(uint16_t port_number, uint32_t data)
{
    __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (port_number));
}

uint32_t port_32bit_read(uint16_t port_number)
{
    uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (port_number));
    return result;
}

