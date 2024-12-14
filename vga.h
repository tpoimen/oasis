#ifndef VGA_H
#define VGA_H

#include <stdint.h>

typedef enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
} vga_color;

/*
 * Function: vga_entry_color
 *
 * fg: foreground color of entry
 * bg: background color of entry
 *
 * returns: uint8_t containing the color portion of a VGA entry
 */
static inline uint8_t vga_entry_color(vga_color fg, vga_color bg)
{
    return fg | bg << 4;
}

/*
 * Function: vga_entry
 *
 * uc: character to place in entry
 * color: foreground and background color to place in entry
 *
 * returns: complete VGA entry with character and colors
 */
static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

#endif  // VGA_H

