#include <stdint.h>
#include <printf.h>
#include <string.h>

#include "amiga.h"
#include "font8x8/font8x8.h"

#define BitplaneSizePAL ((320 / 16) * 256)
#define BitplaneSizeNTSC ((320 / 16) * 200)

uint16_t BitplaneBuffer[4][BitplaneSizePAL];

uint16_t CopperList[] = {
    COPPER_MOVE(0x0E0, 0),
    COPPER_MOVE(0x0E2, 0),
    COPPER_MOVE(0x0E4, 0),
    COPPER_MOVE(0x0E6, 0),
    COPPER_MOVE(0x0E8, 0),
    COPPER_MOVE(0x0EA, 0),
    COPPER_MOVE(0x0EC, 0),
    COPPER_MOVE(0x0EE, 0),
    0xffff, 0xfffe
};
uint16_t ColorPalette[] = {
    0x0000, //0: Black
    0x0C00, //1: Red
    0x00C0, //2: Green
    0x0CC0, //3: Yellow
    0x000C, //4: Blue
    0x0C0C, //5: Magenta
    0x00CC, //6: Cyan
    0x0CCC, //7: Light Gray
    0x0888, //8: Dark Gray
    0x0f00, //9: Bright Red
    0x00f0, //10: Bright Green
    0x0ff0, //11: Bright Yellow
    0x000f, //12: Bright Blue
    0x0f0f, //13: Bright Magenta
    0x00ff, //14: Bright Cyan
    0x0fff, //15: White
};
#define TEXT_BLACK  "\x80"
#define TEXT_RED    "\x81"
#define TEXT_GREEN  "\x82"
#define TEXT_YELLOW "\x83"
#define TEXT_BLUE   "\x84"
#define TEXT_MAGEN  "\x85"
#define TEXT_CYAN   "\x86"
#define TEXT_GRAY   "\x87"
#define TEXT_DGRAY  "\x88"
#define TEXT_BRED   "\x89"
#define TEXT_BGREEN "\x8A"
#define TEXT_BYELLOW"\x8B"
#define TEXT_BBLUE  "\x8C"
#define TEXT_BMAGEN "\x8D"
#define TEXT_BCYAN  "\x8E"
#define TEXT_WHITE  "\x8f"
#define BACK_BLACK  "\x90"
#define BACK_RED    "\x91"
#define BACK_GREEN  "\x92"
#define BACK_YELLOW "\x93"
#define BACK_BLUE   "\x94"
#define BACK_MAGEN  "\x95"
#define BACK_CYAN   "\x96"
#define BACK_GRAY   "\x97"
#define BACK_DGRAY  "\x98"
#define BACK_BRED   "\x99"
#define BACK_BGREEN "\x9A"
#define BACK_BYELLOW"\x9B"
#define BACK_BBLUE  "\x9C"
#define BACK_BMAGEN "\x9D"
#define BACK_BCYAN  "\x9E"
#define BACK_WHITE  "\x9f"


int cursorX = 0;
int cursorY = 0;
void setCusor(int x, int y) {
    if (x < 0) cursorX = 0;
    else if (x >= 40) cursorX = 39;
    else cursorX = x;
    if (y < 0) cursorY = 0;
    else if (y >= 32) cursorY = 31;
    else cursorY = y;
}
int textColor = 15;
int backgroundColor = 0;
inline void setTextColor(int color) {
    textColor = color;
}
inline void setBackgroundColor(int color) {
    backgroundColor = color;
}
unsigned char reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}
void writeChar(unsigned char c) {
    if (c < 0) return;
    if (c > 127) {
        if (c >= 0x80 && c <= 0x8f) setTextColor(c - 0x80);
        if (c >= 0x90 && c <= 0x9f) setBackgroundColor(c - 0x90);
        return;
    }
    for (int i = 0; i < 4; i++) {
        bool fillChar = (textColor & (1 << i)) ? true : false;
        bool fillBack = (backgroundColor & (1 << i)) ? true : false;
        uint8_t* bpl = (uint8_t*) BitplaneBuffer[i];
        bpl += cursorX;
        bpl += (cursorY * 40 * 8);
        if (fillChar && fillBack) {
            //fill bitplane completly
            for (int j = 0; j < 8; j++) {
                *bpl = 0xff;
                bpl += 40;
            }
        } else if (!fillChar && !fillBack) {
            //unfill bitplane completly
            for (int j = 0; j < 8; j++) {
                *bpl = 0x00;
                bpl += 40;
            }
        } else if (fillChar && !fillBack) {
            //fill character
            for (int j = 0; j < 8; j++) {
                *bpl = reverse(font8x8_basic[c][j]);
                bpl += 40;
            }
        } else {
            //fill background
            for (int j = 0; j < 8; j++) {
                *bpl = ~reverse(font8x8_basic[c][j]);
                bpl += 40;
            }
        }
    }
    if (++cursorX >= 40) {
        cursorX = 0;
        if (++cursorY >= 32) {
            //should scroll screen here
            cursorY = 0;
        }
    }
}
void _putchar(char c) {
    unsigned char uc = (unsigned char) c;
    if (uc >= 0x20) writeChar(c);
    switch (uc) {
        case 0x0A: //new line
            cursorX = 0;
            if (++cursorY >= 32) cursorY = 0;
            break;
    }
}
void clearScreen() {
    cursorX = 0;
    cursorY = 0;
    for (int i = 0; i < 4; i++) {
        bool fillPlane = (backgroundColor & (1 << i)) ? true : false;
        memset(BitplaneBuffer[i], fillPlane ? 0xff : 0x00, BitplaneSizePAL * 2);
    }
}

int main() {
    //initialize system as lowrew PAL with no bitplanes enabled
    //set background color to white
    //this should result in a white screen

    uint16_t* CopL = &CopperList[0];
    for (int i = 0; i < 4; i++) {
        CopL++;
        *CopL++ = (uint32_t) BitplaneBuffer[i] >> 16;
        CopL++;
        *CopL++ = (uint32_t) BitplaneBuffer[i] & 0xffff;
    }

    AmigaInitSystem(true, true, 4, (uint16_t**) BitplaneBuffer);
    AmigaChipset->copper.COP1LC = (uint32_t) CopperList;
    for (int i = 0; i < 16; i++) AmigaChipset->COLOR[i] = ColorPalette[i];
    clearScreen();
    printf("Hello World!\nNewline\n");
    printf(TEXT_RED"red text\n"TEXT_GREEN"green text\n"TEXT_BLUE"blue text\n");
    printf(TEXT_BLACK BACK_WHITE "black on white text");
    
    while (true);
    return 0;
}

