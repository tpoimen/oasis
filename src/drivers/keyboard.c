#include <drivers/keyboard.h>
#include <hardwarecommunication/interrupts.h>

#include <stddef.h>

// FIXME: bad practice, deglobalize this
struct keyboard_driver kb;

void keyboard_driver_init(void)
{
    interrupt_manager_set_handler(0x21, keyboard_handle_interrupt);
    kb.command_port = port_init(PORT_8BIT_SLOW, 0x64);
    kb.data_port = port_init(PORT_8BIT_SLOW, 0x60);

    while (port_read(&kb.command_port) & 0x1)
        port_read(&kb.data_port);
    port_write(&kb.command_port, 0xAE);    // activate interrupts
    port_write(&kb.command_port, 0x20);    // get current state
    uint8_t status = (port_read(&kb.data_port) | 1) & ~0x10;
    port_write(&kb.command_port, 0x60);    // set state
    port_write(&kb.data_port, status);

    port_write(&kb.data_port, 0xF4);
}

void keyboard_driver_delete(void)
{
    interrupt_manager_remove_handler(0x21);
}

void printf(const char*);

#define print_keyboard_input(c1, c2) { \
    shift | caps ? printf(c2) : printf(c1); \
    break; \
}

uint32_t keyboard_handle_interrupt(uint32_t esp)
{
    static uint8_t shift = 0;
    static uint8_t caps = 0;

    uint8_t key = port_read(&kb.data_port);
    
    switch (key)
    {
        case 0x02: print_keyboard_input("1", "!");
        case 0x03: print_keyboard_input("2", "@");
        case 0x04: print_keyboard_input("3", "#");
        case 0x05: print_keyboard_input("4", "$");
        case 0x06: print_keyboard_input("5", "%");
        case 0x07: print_keyboard_input("6", "^");
        case 0x08: print_keyboard_input("7", "&");
        case 0x09: print_keyboard_input("8", "*");
        case 0x0A: print_keyboard_input("9", "(");
        case 0x0B: print_keyboard_input("0", ")");
        case 0x0C: print_keyboard_input("-", "_");
        case 0x0D: print_keyboard_input("=", "+");

        case 0x10: print_keyboard_input("q", "Q");
        case 0x11: print_keyboard_input("w", "W");
        case 0x12: print_keyboard_input("e", "E");
        case 0x13: print_keyboard_input("r", "R");
        case 0x14: print_keyboard_input("t", "T");
        case 0x15: print_keyboard_input("y", "Y");
        case 0x16: print_keyboard_input("u", "U");
        case 0x17: print_keyboard_input("i", "I");
        case 0x18: print_keyboard_input("o", "O");
        case 0x19: print_keyboard_input("p", "P");
        case 0x1A: print_keyboard_input("[", "{");
        case 0x1B: print_keyboard_input("]", "}");
        
        case 0x1E: print_keyboard_input("a", "A");
        case 0x1F: print_keyboard_input("s", "S");
        case 0x20: print_keyboard_input("d", "D"); 
        case 0x21: print_keyboard_input("f", "F");
        case 0x22: print_keyboard_input("g", "G");
        case 0x23: print_keyboard_input("h", "H");
        case 0x24: print_keyboard_input("j", "J");
        case 0x25: print_keyboard_input("k", "K");
        case 0x26: print_keyboard_input("l", "L");
        case 0x27: print_keyboard_input(";", ":");
        case 0x28: print_keyboard_input("\'", "\"");
        case 0x29: print_keyboard_input("`", "~");

        case 0x2B: print_keyboard_input("\\", "|");
        case 0x2C: print_keyboard_input("z", "Z");
        case 0x2D: print_keyboard_input("x", "X");
        case 0x2E: print_keyboard_input("c", "C");
        case 0x2F: print_keyboard_input("v", "V");
        case 0x30: print_keyboard_input("b", "B");
        case 0x31: print_keyboard_input("n", "N");
        case 0x32: print_keyboard_input("m", "M");
        case 0x33: print_keyboard_input(",", "<");
        case 0x34: print_keyboard_input(".", ">");
        case 0x35: print_keyboard_input("/", "?");

        case 0x39: printf(" "); break;
        case 0x0E: char c = (char)0x7f; printf(&c); break;
        case 0x0F: printf("    "); break;

        case 0x1C: break;  // enter pressed
        case 0x9C: printf("\n"); break;
        
        case 0x36:
        case 0x2A: shift = 1; break;
        case 0xB6:
        case 0xAA: shift = 0; break;
        case 0x3A: break;  // caps pressed
        case 0xBA: caps = caps == 0 ? 1 : 0; break;

        default:
            if (key >= 0x80) break;
            char* foo = "KEYBOARD 0x00 ";
            char* hex = "0123456789ABCDEF";
            foo[11] = hex[(key >> 4) & 0x0F];
            foo[12] = hex[key & 0x0F];
            printf(foo);
    }
    return esp;
}

