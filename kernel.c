
void printf(char* str)
{
    unsigned short* vga_buffer = (unsigned short *) 0xb8000;

    for (int i = 0; str[i] != '\0'; ++i)
    {
        vga_buffer[i] = (vga_buffer[i] & 0xFF00) | str[i];
    }
}

void kernel_main(void* multiboot_structure, unsigned int magic_number)
{
    printf("Hello from Oasis kernel!");

    while(1);
}

