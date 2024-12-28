#include <drivers/vga.h>
#include <gdt.h>
#include <hardwarecommunication/port.h>
#include <hardwarecommunication/interrupts.h>

void printf(char* str)
{
    unsigned short* vga_buffer = (unsigned short *) 0xb8000;
    static uint8_t x = 0;
    static uint8_t y = 0;
    const uint8_t WIDTH = 80;
    const uint8_t HEIGHT = 25;

    uint8_t color = vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
    for (int i = 0; str[i] != '\0'; ++i)
    {
        if (str[i] == '\n' || x == WIDTH) {
            x = 0;
            y = (y == HEIGHT - 1) ? 0 : ++y;
            if (str[i] == '\n') continue;
        }
        
        vga_buffer[y * WIDTH + x] = vga_entry(str[i], color);
        x++;
    }
}

void kernel_main(void* multiboot_structure, unsigned int magic_number)
{
    printf("Hello from Oasis kernel!\nThis should be on a new line\n");    

    gdt_init();
    interrupt_manager_init();
    
    activate_interrupts();

    while(1);
}

