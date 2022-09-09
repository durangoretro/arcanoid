#ifndef _H_DLIB
#define _H_DLIB


//Colours
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

/* Draw procedures */
extern void __fastcall__ fillScreen(byte color);

/* Debug procedures */
extern void __fastcall__ consoleLogHex(void);
extern void __fastcall__ consoleLogBinary(void);
extern void __fastcall__ consoleLogDecimal(void);
extern void __fastcall__ consoleLogChar(void);
extern void __fastcall__ consoleLogStr(void);
extern void __fastcall__ startStopwatch(void);
extern void __fastcall__ stopStopwatch(void);

#endif
