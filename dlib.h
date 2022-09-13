#ifndef _H_DLIB
#define _H_DLIB


// Colours
#define BLACK 0x00
#define GREEN 0x11
#define RED 0x22
#define ORANGE 0x33
#define PHARMACY_GREEN 0x44
#define LIME 0x55
#define MYSTIC_RED 0x66
#define YELLOW 0x77
#define BLUE 0x88
#define DEEP_SKY_BLUE 0x99
#define MAGENTA 0xaa
#define LAVENDER_ROSE 0xbb
#define NAVY_BLUE 0xcc
#define CIAN 0xdd
#define PINK_FLAMINGO 0xee
#define WHITE 0xff

// Gamepad keys
#define BUTTON_A 0x80
#define BUTTON_START 0x40
#define BUTTON_B 0x20
#define BUTTON_SELECT 0x10
#define BUTTON_UP 0x08
#define BUTTON_LEFT 0x04
#define BUTTON_DOWN 0x02
#define BUTTON_RIGHT 0x01

/* type definitions */
typedef unsigned char byte;
typedef unsigned short word;

typedef struct{
    byte x, y;
    word mem;
    byte color;
    byte width, height;
} rectangle;

/* System procedures */
extern void __fastcall__ waitVSync(void);
extern void __fastcall__ waitStart(void);
extern void __fastcall__ waitFrames(byte);
extern byte __fastcall__ readGamepad(byte);

/* Draw procedures */
extern void __fastcall__ fillScreen(byte color);
extern void __fastcall__ drawRect(void*);
extern void __fastcall__ moveRight(void*);
extern void __fastcall__ moveLeft(void*);

/* Debug procedures */
extern void __fastcall__ consoleLogHex(byte value);
extern void __fastcall__ consoleLogWord(word value);
extern void __fastcall__ consoleLogBinary(byte value);
extern void __fastcall__ consoleLogDecimal(byte value);
extern void __fastcall__ consoleLogChar(unsigned char);
extern void __fastcall__ consoleLogStr(char *str);
extern void __fastcall__ startStopwatch(void);
extern void __fastcall__ stopStopwatch(void);

#endif
