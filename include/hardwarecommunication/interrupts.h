#ifndef _OASIS_HARDWARECOMMUNICATION_INTERRUPTS_H_
#define _OASIS_HARDWARECOMMUNICATION_INTERRUPTS_H_

#include <stdint.h>

#include <hardwarecommunication/port.h>
#include <gdt.h>

#define NUM_GATE_DESCRIPTOR_ENTRIES 256

struct gate_descriptor
{
    uint16_t handler_address_low_bits;
    uint16_t gdt_code_segment_selector;
    uint8_t reserved;
    uint8_t access;
    uint16_t handler_address_high_bits;
} __attribute__((packed)); 

struct interrupt_descriptor_table_pointer
{
    uint16_t size;
    uint32_t base;
} __attribute__((packed));

struct interrupt_manager
{
    struct gate_descriptor idt[NUM_GATE_DESCRIPTOR_ENTRIES];
    struct interrupt_descriptor_table_pointer idt_ptr;
    port pic_primary_command;
    port pic_primary_data;
    port pic_secondary_command;
    port pic_secondary_data;
} __attribute__((packed));

void set_interrupt_descriptor_table_entry(
    uint8_t interrupt_number,
    uint16_t code_segment_selector_offset,
    void (*handler)(),
    uint8_t descriptor_privilege_level,
    uint8_t desriptor_type
);

void interrupt_manager_init(void);

void activate_interrupts(void);
void deactivate_interrupts(void);

uint32_t handle_interrupt(uint8_t interrupt_number, uint32_t esp);

void ignore_interrupt_request(void);
void handle_interrupt_request_0x00(void);
void handle_interrupt_request_0x01(void);

#endif // _OASIS_HARDWARECOMMUNICATION_INTERRUPTS_H_
