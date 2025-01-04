#include <drivers/vga.h>
#include <gdt.h>
#include <hardwarecommunication/port.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/keyboard.h>

void printf(char* str)
{
    unsigned short* vga_buffer = (unsigned short *) 0xb8000;
    static uint8_t x = 0;
    static uint8_t y = 0;
    const uint8_t WIDTH = 80;
    const uint8_t HEIGHT = 25;
    
    uint8_t color = vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
    
    // backspace handling
    if (str[0] == 0x7f) {
        if (y == 0 && x == 0) return;

        if (x == 0) {
            x = WIDTH - 1;
            y--;
        } else {
            x--;
        }
        vga_buffer[y * WIDTH + x] = vga_entry(' ', color);
        return;
    }

    for (int i = 0; str[i] != '\0'; ++i)
    {
        if (str[i] == '\n' || x == WIDTH) {
            x = 0;
            if (y != HEIGHT - 1) {
                ++y;
            } else {
                // move every line up one line (acts like scrolling)
                for (int j = 0; j < WIDTH; j++) {
                    for (int z = 0; z < HEIGHT - 1; z++) {
                        int idx = z * WIDTH + j;
                        vga_buffer[idx] = vga_buffer[idx + WIDTH];
                    }
                }
                
                // clear last row before writing
                for (int j = 0; j < WIDTH; j++)
                    vga_buffer[j + (HEIGHT - 1) * WIDTH] = vga_entry(' ', color);
            }
            if (str[i] == '\n') continue;
        }
        
        vga_buffer[y * WIDTH + x] = vga_entry(str[i], color);
        x++;
    }
}

void kernel_main(void* multiboot_structure, unsigned int magic_number)
{
    printf("Hello from Oasis kernel!\n");    

    gdt_init();
    interrupt_manager_init();
    
    keyboard_driver_init();

    activate_interrupts();

    while(1);
}

