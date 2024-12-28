#include <gdt.h>

// global gdt
struct global_descriptor_table gdt;

void gdt_init(void)
{
    segment_descriptor_init(&gdt.null_segment_selector, 0, 0, 0);
    segment_descriptor_init(&gdt.unused_segment_selector, 0, 0, 0);
    segment_descriptor_init(&gdt.code_segment_selector, 0, 64*1024*1024, 0x9A);
    segment_descriptor_init(&gdt.data_segment_selector, 0, 64*1024*1024, 0x92);
    
    uint32_t i[2];
    i[0] = sizeof(struct global_descriptor_table) << 16; 
    i[1] = (uint32_t)&gdt;
    __asm__ volatile("lgdt (%0)" : : "p" (((uint8_t*)i) + 2));
}

void gdt_delete(void)
{
    return;
}

uint16_t gdt_data_segment_selector(void)
{
    return ((uint8_t*)&gdt.data_segment_selector) - ((uint8_t*)&gdt);
}

uint16_t gdt_code_segment_selector(void)
{
    return ((uint8_t*)&gdt.code_segment_selector) - ((uint8_t*)&gdt);
}

void segment_descriptor_init(struct segment_descriptor* segment, uint32_t base, uint32_t limit, uint8_t flags)
{
    uint8_t* target = (uint8_t*)segment;

    if (limit <= (1<<16))
    {
        target[6] = 0x40;
    }
    else
    {
        if ((limit & 0xFFF) != 0xFFF)
            limit = (limit >> 12) - 1;
        else
            limit = (limit >> 12);
        
        target[6] = 0xC0;
    }
    
    // encode limit
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF;
    
    // encode pointer
    target[2] = (base) & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    // set access rights
    target[5] = flags;
}

uint32_t segment_descriptor_base(struct segment_descriptor* segment)
{
    uint8_t* target = (uint8_t*)segment;
    uint32_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];
    return result;
}

uint32_t segment_descriptor_limit(struct segment_descriptor* segment)
{
    uint8_t* target = (uint8_t*)segment;
    uint32_t result = target[6] & 0xF;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];
    
    if ((target[6] & 0xC0) == 0xC0)
        result = (result << 12) | 0xFFF;
    
    return result;
}

