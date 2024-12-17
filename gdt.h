#ifndef _GDT_H_
#define _GDT_H_

#include <stdint.h>

// Segments

struct segment_descriptor
{
    uint16_t limit_lo;
    uint16_t base_lo;
    uint8_t base_hi;
    uint8_t type;
    uint8_t flags_limit_hi;
    uint8_t base_vhi;
} __attribute__((packed));

void segment_descriptor_init(struct segment_descriptor* segment, uint32_t base, uint32_t limit, uint8_t type);

uint32_t segment_descriptor_base(struct segment_descriptor* segment);
uint32_t segment_descriptor_limit(struct segment_descriptor* segment);

// GDT

struct global_descriptor_table
{
    struct segment_descriptor null_segment_selector;
    struct segment_descriptor unused_segment_selector;
    struct segment_descriptor code_segment_selector;
    struct segment_descriptor data_segment_selector;
} __attribute__((packed));

void gdt_init(void);
void gdt_delete(void);

uint16_t gdt_code_segment_selector(void);
uint16_t gdt_data_segment_selector(void);

#endif  // _GDT_H_

