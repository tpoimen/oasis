#include <hardwarecommunication/interrupts.h>

#include <stddef.h>

void printf(char* str);

// global interrupt manager
struct interrupt_manager interrupt_manager;

void set_interrupt_descriptor_table_entry(
    uint8_t interrupt_number,
    uint16_t code_segment_selector_offset,
    void (*handler)(),
    uint8_t descriptor_privilege_level,
    uint8_t descriptor_type)
{
    const uint8_t IDT_DESC_PRESENT = 0x80;

    struct gate_descriptor *interrupt = &interrupt_manager.idt[interrupt_number];
    interrupt->handler_address_low_bits = ((uint32_t)handler) & 0xFFFF;
    interrupt->handler_address_high_bits = ((uint32_t)handler >> 16) & 0xFFFF;
    interrupt->gdt_code_segment_selector = code_segment_selector_offset;
    interrupt->access = IDT_DESC_PRESENT | descriptor_type | ((descriptor_privilege_level & 3) << 5);
    interrupt->reserved = 0;
}

void interrupt_manager_init(void)
{
    interrupt_manager.pic_primary_command = port_init(PORT_8BIT_SLOW, 0x20);
    interrupt_manager.pic_primary_data = port_init(PORT_8BIT_SLOW, 0x21);
    interrupt_manager.pic_secondary_command = port_init(PORT_8BIT_SLOW, 0xA0);
    interrupt_manager.pic_secondary_data = port_init(PORT_8BIT_SLOW, 0xA1);

    uint16_t code_segment = gdt_code_segment_selector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    
    // define all interrupts to an empty handler. will assign handlers as we go.
    for (uint16_t i = 0; i < NUM_GATE_DESCRIPTOR_ENTRIES; ++i)
    {
        set_interrupt_descriptor_table_entry(
            i,
            code_segment,
            &ignore_interrupt_request, 
            0,                     // kernel space
            IDT_INTERRUPT_GATE
        );
        interrupt_manager.handlers[i] = NULL;
    }

    set_interrupt_descriptor_table_entry(0x20, code_segment, &handle_interrupt_request_0x00, 0, IDT_INTERRUPT_GATE);
    set_interrupt_descriptor_table_entry(0x21, code_segment, &handle_interrupt_request_0x01, 0, IDT_INTERRUPT_GATE);
    
    port_write(&interrupt_manager.pic_primary_command, 0x11);
    port_write(&interrupt_manager.pic_secondary_command, 0x11);
    
    port_write(&interrupt_manager.pic_primary_data, 0x20);        // interrupts 0x20-0x27
    port_write(&interrupt_manager.pic_secondary_data, 0x28);      // interrupts 0x28-0x30

    port_write(&interrupt_manager.pic_primary_data, 0x04);
    port_write(&interrupt_manager.pic_secondary_data, 0x02);

    port_write(&interrupt_manager.pic_primary_data, 0x01);
    port_write(&interrupt_manager.pic_secondary_data, 0x01);

    port_write(&interrupt_manager.pic_primary_data, 0x00);
    port_write(&interrupt_manager.pic_secondary_data, 0x00);
    
    // load idt for CPU to use
    interrupt_manager.idt_ptr.size = NUM_GATE_DESCRIPTOR_ENTRIES * sizeof(struct gate_descriptor) - 1;
    interrupt_manager.idt_ptr.base = (uint32_t)(interrupt_manager.idt);
    __asm__ volatile("lidt %0" : : "m" (interrupt_manager.idt_ptr));
}

void activate_interrupts(void)
{
    __asm__ volatile("sti");
}

void deactivate_interrupts(void)
{
    __asm__ volatile("cli");
}

void interrupt_manager_set_handler(uint8_t interrupt_number, uint32_t (*handler)(uint32_t))
{
    interrupt_manager.handlers[interrupt_number] = handler;
}

void interrupt_manager_remove_handler(uint8_t interrupt_number)
{
    interrupt_manager.handlers[interrupt_number] = NULL;
}

uint32_t handle_interrupt(uint8_t interrupt_number, uint32_t esp)
{
    if (interrupt_manager.handlers[interrupt_number] != NULL) {
        esp = interrupt_manager.handlers[interrupt_number](esp);
    }
    else if (interrupt_number != 0x20) // don't want to print timer interrupts for now.
    {
        char* foo = "UNHANDLED INTERRUPT 0x00";
        char* hex = "0123456789ABCDEF";
        foo[22] = hex[(interrupt_number >> 4) & 0x0F];
        foo[23] = hex[interrupt_number & 0x0F];
        printf(foo);
    }

    // if hardware interrupts, tell PICs so they know we are done handling the request
    if (0x20 <= interrupt_number && interrupt_number < 0x30) {
        port_write(&interrupt_manager.pic_primary_command, 0x20);
        if (0x28 <= interrupt_number)
            port_write(&interrupt_manager.pic_secondary_command, 0x20);
    }

    return esp;
}

