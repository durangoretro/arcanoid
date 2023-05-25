#ifndef _H_DLIB
#define _H_DLIB

/* type definitions */
typedef unsigned char byte;
typedef unsigned short word;

typedef struct{
    byte x, y;
    word mem;
    byte color;
    byte width, height;
} rectangle;

typedef struct{
    byte x, y;
    word mem;
    byte color;
    byte width, height;
    byte enabled;
    byte x2,y2;
} brick;

typedef struct{
    byte x,y;
    word mem;
    byte color;
    byte vx, vy;
} ball;



/* Draw procedures */
extern void __fastcall__ fillScreen(byte color);
extern void __fastcall__ drawRect(void*);
extern void __fastcall__ drawBall(void*);
extern void __fastcall__ moveBall(void*);
extern void __fastcall__ cleanBall(void*);
extern void __fastcall__ moveRight(void*);
extern void __fastcall__ moveLeft(void*);

#endif
