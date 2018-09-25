#include <string.h> /* memcpy */
#include <stdint.h> /* uint8_t, uint16_t */

#include "salzalon.h"

#define SCREEN_PIXEL_WIDTH  280
#define SCREEN_PIXEL_HEIGHT 192
#define CHAR_PIXEL_WIDTH  7
#define CHAR_PIXEL_HEIGHT 8

/* Soft switches */
#define EIGHTY_COL_STORE_OFF ((uint8_t *) 0xc000)
#define EIGHTY_COL_STORE_ON  ((uint8_t *) 0xc001)

#define RAMRDOFF ((uint8_t *) 0xc002)
#define RAMRDON  ((uint8_t *) 0xc003)
#define RAMWTOFF ((uint8_t *) 0xc004)
#define RAMWTON  ((uint8_t *) 0xc005)
#define SET80COL ((uint8_t *) 0xc00d)
#define TEXTOFF  ((uint8_t *) 0xc050)
#define MIXEDON  ((uint8_t *) 0xc053)
#define PAGE2OFF ((uint8_t *) 0xc054)
#define PAGE2ON  ((uint8_t *) 0xc055)
#define HIRESOFF ((uint8_t *) 0xc056)
#define HIRESON  ((uint8_t *) 0xc057)
#define SETAN3   ((uint8_t *) 0xc05e)

/* Subroutines */
#define TEXT     ((uint8_t *) 0xF399)
#define HGR2     ((uint8_t *) 0Xf3d8)
#define CLRHGR   ((uint8_t *) 0XF3E2)

#define ADDR uint8_t*

uint8_t *yAddressTable[] = {
(uint8_t *) 0x2000, (uint8_t *) 0x2400, (uint8_t *) 0x2800, (uint8_t *) 0x2c00, 
(uint8_t *) 0x3000, (uint8_t *) 0x3400, (uint8_t *) 0x3800, (uint8_t *) 0x3c00, 
(uint8_t *) 0x2080, (uint8_t *) 0x2480, (uint8_t *) 0x2880, (uint8_t *) 0x2c80, 
(uint8_t *) 0x3080, (uint8_t *) 0x3480, (uint8_t *) 0x3880, (uint8_t *) 0x3c80, 
(uint8_t *) 0x2100, (uint8_t *) 0x2500, (uint8_t *) 0x2900, (uint8_t *) 0x2d00, 
(uint8_t *) 0x3100, (uint8_t *) 0x3500, (uint8_t *) 0x3900, (uint8_t *) 0x3d00, 
(uint8_t *) 0x2180, (uint8_t *) 0x2580, (uint8_t *) 0x2980, (uint8_t *) 0x2d80, 
(uint8_t *) 0x3180, (uint8_t *) 0x3580, (uint8_t *) 0x3980, (uint8_t *) 0x3d80, 
(uint8_t *) 0x2200, (uint8_t *) 0x2600, (uint8_t *) 0x2a00, (uint8_t *) 0x2e00, 
(uint8_t *) 0x3200, (uint8_t *) 0x3600, (uint8_t *) 0x3a00, (uint8_t *) 0x3e00, 
(uint8_t *) 0x2280, (uint8_t *) 0x2680, (uint8_t *) 0x2a80, (uint8_t *) 0x2e80, 
(uint8_t *) 0x3280, (uint8_t *) 0x3680, (uint8_t *) 0x3a80, (uint8_t *) 0x3e80, 
(uint8_t *) 0x2300, (uint8_t *) 0x2700, (uint8_t *) 0x2b00, (uint8_t *) 0x2f00, 
(uint8_t *) 0x3300, (uint8_t *) 0x3700, (uint8_t *) 0x3b00, (uint8_t *) 0x3f00, 
(uint8_t *) 0x2380, (uint8_t *) 0x2780, (uint8_t *) 0x2b80, (uint8_t *) 0x2f80, 
(uint8_t *) 0x3380, (uint8_t *) 0x3780, (uint8_t *) 0x3b80, (uint8_t *) 0x3f80, 
(uint8_t *) 0x2028, (uint8_t *) 0x2428, (uint8_t *) 0x2828, (uint8_t *) 0x2c28, 
(uint8_t *) 0x3028, (uint8_t *) 0x3428, (uint8_t *) 0x3828, (uint8_t *) 0x3c28, 
(uint8_t *) 0x20a8, (uint8_t *) 0x24a8, (uint8_t *) 0x28a8, (uint8_t *) 0x2ca8, 
(uint8_t *) 0x30a8, (uint8_t *) 0x34a8, (uint8_t *) 0x38a8, (uint8_t *) 0x3ca8, 
(uint8_t *) 0x2128, (uint8_t *) 0x2528, (uint8_t *) 0x2928, (uint8_t *) 0x2d28, 
(uint8_t *) 0x3128, (uint8_t *) 0x3528, (uint8_t *) 0x3928, (uint8_t *) 0x3d28, 
(uint8_t *) 0x21a8, (uint8_t *) 0x25a8, (uint8_t *) 0x29a8, (uint8_t *) 0x2da8, 
(uint8_t *) 0x31a8, (uint8_t *) 0x35a8, (uint8_t *) 0x39a8, (uint8_t *) 0x3da8, 
(uint8_t *) 0x2228, (uint8_t *) 0x2628, (uint8_t *) 0x2a28, (uint8_t *) 0x2e28, 
(uint8_t *) 0x3228, (uint8_t *) 0x3628, (uint8_t *) 0x3a28, (uint8_t *) 0x3e28, 
(uint8_t *) 0x22a8, (uint8_t *) 0x26a8, (uint8_t *) 0x2aa8, (uint8_t *) 0x2ea8, 
(uint8_t *) 0x32a8, (uint8_t *) 0x36a8, (uint8_t *) 0x3aa8, (uint8_t *) 0x3ea8, 
(uint8_t *) 0x2328, (uint8_t *) 0x2728, (uint8_t *) 0x2b28, (uint8_t *) 0x2f28, 
(uint8_t *) 0x3328, (uint8_t *) 0x3728, (uint8_t *) 0x3b28, (uint8_t *) 0x3f28, 
(uint8_t *) 0x23a8, (uint8_t *) 0x27a8, (uint8_t *) 0x2ba8, (uint8_t *) 0x2fa8, 
(uint8_t *) 0x33a8, (uint8_t *) 0x37a8, (uint8_t *) 0x3ba8, (uint8_t *) 0x3fa8, 
(uint8_t *) 0x2050, (uint8_t *) 0x2450, (uint8_t *) 0x2850, (uint8_t *) 0x2c50, 
(uint8_t *) 0x3050, (uint8_t *) 0x3450, (uint8_t *) 0x3850, (uint8_t *) 0x3c50, 
(uint8_t *) 0x20d0, (uint8_t *) 0x24d0, (uint8_t *) 0x28d0, (uint8_t *) 0x2cd0, 
(uint8_t *) 0x30d0, (uint8_t *) 0x34d0, (uint8_t *) 0x38d0, (uint8_t *) 0x3cd0, 
(uint8_t *) 0x2150, (uint8_t *) 0x2550, (uint8_t *) 0x2950, (uint8_t *) 0x2d50, 
(uint8_t *) 0x3150, (uint8_t *) 0x3550, (uint8_t *) 0x3950, (uint8_t *) 0x3d50, 
(uint8_t *) 0x21d0, (uint8_t *) 0x25d0, (uint8_t *) 0x29d0, (uint8_t *) 0x2dd0, 
(uint8_t *) 0x31d0, (uint8_t *) 0x35d0, (uint8_t *) 0x39d0, (uint8_t *) 0x3dd0, 
(uint8_t *) 0x2250, (uint8_t *) 0x2650, (uint8_t *) 0x2a50, (uint8_t *) 0x2e50, 
(uint8_t *) 0x3250, (uint8_t *) 0x3650, (uint8_t *) 0x3a50, (uint8_t *) 0x3e50, 
(uint8_t *) 0x22d0, (uint8_t *) 0x26d0, (uint8_t *) 0x2ad0, (uint8_t *) 0x2ed0, 
(uint8_t *) 0x32d0, (uint8_t *) 0x36d0, (uint8_t *) 0x3ad0, (uint8_t *) 0x3ed0, 
(uint8_t *) 0x2350, (uint8_t *) 0x2750, (uint8_t *) 0x2b50, (uint8_t *) 0x2f50, 
(uint8_t *) 0x3350, (uint8_t *) 0x3750, (uint8_t *) 0x3b50, (uint8_t *) 0x3f50, 
(uint8_t *) 0x23d0, (uint8_t *) 0x27d0, (uint8_t *) 0x2bd0, (uint8_t *) 0x2fd0, 
(uint8_t *) 0x33d0, (uint8_t *) 0x37d0, (uint8_t *) 0x3bd0, (uint8_t *) 0x3fd0, 
};

uint8_t capY[] = {
    0x22,
    0x92,
    0x14,
    0x8c,
    0x08,
    0x08,
    0x08,
    0x00
};

struct Font
{
    uint8_t pixelWidth;
    uint8_t pixelHeight;
    uint8_t glyphCount;
    uint8_t **glyphs;
};

struct Font blah = { 7, 12, 6, { {0} } };

uint8_t cursorX = 0;
uint8_t cursorY = 0;
struct Font *font;

void drawChar(char c)
{
    uint8_t screenPixelY;
    uint8_t charPixelY;

    for (charPixelY = 0, screenPixelY = cursorY * CHAR_PIXEL_HEIGHT;
            charPixelY < CHAR_PIXEL_HEIGHT;
            ++charPixelY, ++screenPixelY) {
        /* Transfer one byte to memory representing 7 horizontal pixels
           at vertical position Y */
        *(yAddressTable[screenPixelY] + cursorX) = salzalon[c][charPixelY];
    }

    if (cursorX < 40) {
        ++cursorX;
    }
    else {
        cursorX = 0;
        ++cursorY;
    }
}

void main(void)
{
    uint8_t x = 10;
    uint8_t y = 50;
    char c;

    /* Set soft switches to turn on HGR page 1. */
    *TEXTOFF  = (uint8_t) 1;
    *HIRESON  = (uint8_t) 1;
    *MIXEDON  = (uint8_t) 1;
    *PAGE2OFF = (uint8_t) 1;

    for (c = '!'; c <= 'd'; ++c)
        drawChar(c);

/*
    for (i = 0; i < 16; ++i) {
        *(yAddressTable[y + i] + x) = salzalon[i];
    }
    ++x;
    for (i = 0; i < 16; ++i) {
        *(yAddressTable[y + i] + x) = runes_as_ur[i];
    }
    ++x;
    for (i = 0; i < 16; ++i) {
        *(yAddressTable[y + i] + x) = runes_as_thorn[i];
    }
    x=10;
    y+=16;
    for (i = 0; i < 12; ++i) {
        *(yAddressTable[y + i] + x) = salzalon[i];
    }
    ++x;
    for (i = 0; i < 12; ++i) {
        *(yAddressTable[y + i] + x) = salzalon[i];
    }
    ++x;
    for (i = 0; i < 12; ++i) {
        *(yAddressTable[y + i] + x) = salzalon[i];
    }
*/
}

