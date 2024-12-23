#include <drivers/vga.h>
#include <gdt.h>
#include <hardwarecommunication/port.h>

void printf(char* str)
{
    unsigned short* vga_buffer = (unsigned short *) 0xb8000;

    uint8_t color = vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
    for (int i = 0; str[i] != '\0'; ++i)
    {
        vga_buffer[i] = vga_entry(str[i], color);
    }
}

void kernel_main(void* multiboot_structure, unsigned int magic_number)
{
    printf("Hello from Oasis kernel!");    

    gdt_init();

    port example = port_init(PORT_8BIT, 0);
    port_write(&example, 0);

    while(1);
}

