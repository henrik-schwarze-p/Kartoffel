//
//  Target.cpp
//  Kartoffel
//
//  Created by Kurt Schwarze on 16.03.19.
//  Copyright © 2019 Kurt Schwarze. All rights reserved.
//

#ifdef MEGA
#include "Target.h"
#include <stdio.h>

#include "Kartoffel.h"
#include "EEPROM.h"

TFT tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void tftReset() {
    tft.reset();
    tft.begin(0x9341);
}

void tftRect(int x, int y, int w, int h, int c) {
    tft.fillRect(x, y, w, h, c);
}

void tftPixel(int x, int y, int c) {
    tft.drawPixel(x, y, c);
}

void tftVline(int x, int y, int w, int c) {
    tft.drawFastVLine(x, y, w, c);
}

void tfHline(int x, int y, int l, int c) {
    tft.drawFastHLine(x, y, l, c);
}

// TOUCHING

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
extern TFT  tft;

int           mouseEventX = 0;
int           mouseEventY = 0;
unsigned char mouseState = 0;
unsigned char wasTouched = 0;
unsigned long lastTouch = 0;

/**
 Returns true if a touch happened.
 */
int listenToTouchesOnScreen() {
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    int isTouching = p.z >= 10 && p.z <= 1000;

    if (isTouching) {
        lastTouch = millis();
        if (mouseState == 0) {
            mouseEventY = tft.width() - map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
            mouseEventX = tft.height() - map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
            wasTouched = 1;
            mouseState = 1;
            return 1;
        }
    } else if (mouseState == 1) {
        if (millis() - lastTouch > 300)
            mouseState = 0;
    }
    return 0;
}

/**
  Returns true if a touch happened since last call
 */
int touched() {
    listenToTouchesOnScreen();
    int result = wasTouched;
    wasTouched = 0;
    return result;
    /*
    int result = mouseUp;
    if (mouseUp) {
        mouseUp = 0;
        mouseDown = 0;
    }
    return result;
     */
}

int getTouchX() {
    return mouseEventX;
}

int getTouchY() {
    return mouseEventY;
}

// CONSOLE

int serialized = 0;

void ensureSerial() {
    // if (!serialized)  {
    //     Serial.begin(9600);
    //     serialized=1;
    // }
}

// CONSOLE

void console(const char* c) {
    ensureSerial();
    //    Serial.print(c);
}

void console(float f) {
    ensureSerial();
    // Serial.print(f);
}

void console(int i) {
    ensureSerial();
    // Serial.print(i);
}

void consoleln(const char* c) {
    ensureSerial();
    // Serial.println(c);
}

void consoleln(unsigned long c) {
    ensureSerial();
    // Serial.println(c);
}

void consoleln(float f) {
    ensureSerial();
    // Serial.println(f);
}

void consoleln(int i) {
    ensureSerial();
    // Serial.println(i);
}

void consoleln() {
    ensureSerial();
}

// FINISHING

void terminate() {
    for (;;)
        ;
}

// RAM

void writeToHeap(int a, unsigned char c) {
    // nothing done
}

// EPROM

void writeByteToEEPROM(int address, unsigned char value) {
    if (readByteFromEEPROM(address) == value) {
        return;
    }
    if (address < 0 || address >= EPROM_SIZE)
        fatalError(17, address);
    EEPROM.write(address, value);
}

void writeIntToEEPROM(int address, unsigned int value) {
    writeByteToEEPROM(address, value / 256);
    writeByteToEEPROM(address + 1, value % 256);
}

unsigned char readByteFromEEPROM(int address) {
    if (address < 0 || address >= EPROM_SIZE)
        fatalError(4944, address);
    return EEPROM.read(address);
}

unsigned int readIntFromEEPROM(int address) {
    return readByteFromEEPROM(address) * 256 + readByteFromEEPROM(address + 1);
}

int random(int r) {
    return rand() % r;
}
#elif defined(EMCC)
#include "Target.h"

#include <SDL2/SDL.h>

#include "Kartoffel.h"

extern SDL_Surface* surface;
extern SDL_Window*  window;

// TFT SCREEN

void tftReset() {
}

void tftRect(int x, int y, int w, int h, int c) {
    int rh = w;
    int rw = h;
    int rx = HR - y - h;
    int ry = x;
    int r = (c >> 11) * 255 / 31;
    int g = ((c >> 5) % 64) * 255 / 63;
    int b = (c % 32) * 255 / 31;
    SDL_LockSurface(surface);
    SDL_Rect rect;
    rect.x = rx;
    rect.y = ry;
    rect.w = rw;
    rect.h = rh;
    SDL_FillRect(surface, &rect, SDL_MapRGBA(surface->format, r, g, b, 255));
    SDL_UnlockSurface(surface);
    // SDL_UpdateWindowSurface(window);
    // usleep(w * h / 1000);
}

void _setPixel(int x, int y, int color) {
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (x > 959)
        x = 959;
    if (y > 240 * 3 - 1)
        y = 240 * 3 - 1;
    int r = (color >> 11) * 255 / 31;
    int g = ((color >> 5) % 64) * 255 / 63;
    int b = (color % 32) * 255 / 31;
    SDL_LockSurface(surface);
    *((Uint32*)surface->pixels + y * 320 * 3 + x) = SDL_MapRGBA(surface->format, r, g, b, 255);
    SDL_UnlockSurface(surface);
}

void tftPixel(int x, int y, int c) {
    _setPixel(HR - y - 1, x, c);
}

void tftVline(int x, int y, int w, int c) {
    for (int i = 0; i < w; i++)
        _setPixel(HR - y - i - 1, x, c);
}

void tfHline(int x, int y, int l, int c) {
    for (int i = 0; i < l; i++)
        _setPixel(HR - y - 1, x + i, c);
}

// TOUCH SCREEN

int listenToTouchesOnScreen() {
    return 0;
}

int           px = 0;
int           py = 0;
unsigned char mouseDown = 0;
unsigned char mouseUp = 0;

int touched() {
    listenToTouchesOnScreen();
    int result = mouseUp;
    if (mouseUp) {
        mouseUp = 0;
        mouseDown = 0;
    }
    return result;
}

int getTouchX() {
    return px;
}

int getTouchY() {
    return py;
}

void setTouchX(int x) {
    px = x;
}

void setTouchY(int y) {
    py = y;
}

void setMouseUp(int up) {
    mouseUp = up;
}

// CONSOLE

void console(const char* c) {
    printf("%s", c);
}

void console(float f) {
    printf("%f", f);
}

void console(int i) {
    printf("%d", i);
}

void consoleln(const char* c) {
    printf("%s\n", c);
}

void consoleln(unsigned long c) {
    printf("%lu\n", c);
}

void consoleln(float f) {
    printf("%f\n", f);
}

void consoleln(int i) {
    printf("%d\n", i);
}

void consoleln() {
    printf("\n");
}

// STATIC READING

void useStaticStrings() {
}

void useDynamicStrings() {
}

char pgm_read_byte(const char* a) {
    return *a;
}

const int pgm_read_word_near(const int* a) {
    return *a;
}

char pgm_read_byte_near(const unsigned char* a) {
    return *a;
}

const char* PSTR(const char* a) {
    return a;
}

// FINISHING

void terminate() {
    exit(1);
}

// RAM

int heap[10000];
int _eprom[10000];

void writeToHeap(int a, unsigned char c) {
    heap[a] = c;
}

// EPROM

FILE* outfile = NULL;

void writeByteToEEPROM(int a, unsigned char c) {
    if (a < 0 || a >= EPROM_SIZE)
        fatalError(8, a);
    if (outfile == NULL) {
        unsigned char c[4096];
        for (int i = 0; i < 4096; i++)
            c[i] = 0;
        outfile = fopen("/file.txt", "wb");
        fwrite(&c, 1, 4096, outfile);
    }
    fseek(outfile, a, SEEK_SET);
    fwrite(&c, 1, 1, outfile);
    fflush(outfile);
    _eprom[a] = c;
}

void writeIntToEEPROM(int address, unsigned int value) {
    writeByteToEEPROM(address, value / 256);
    writeByteToEEPROM(address + 1, value % 256);
}

unsigned char readByteFromEEPROM(int address) {
    if (address < 0 || address >= EPROM_SIZE)
        fatalError(18, address);
    return (unsigned char)_eprom[address];
}

unsigned int readIntFromEEPROM(int address) {
    return readByteFromEEPROM(address) * 256 + readByteFromEEPROM(address + 1);
}

int random(int r) {
    return rand() % r;
}

#else

#include "Target.h"

#include <SDL2/SDL.h>

#include "Kartoffel.h"

extern SDL_Surface* surface;
extern SDL_Window*  window;

// TFT SCREEN

void tftReset() {
}

void tftRect(int x, int y, int w, int h, int c) {
    int rh = w;
    int rw = h;
    int rx = HR - y - h;
    int ry = x;
    int r = (c >> 11) * 255 / 31;
    int g = ((c >> 5) % 64) * 255 / 63;
    int b = (c % 32) * 255 / 31;
    SDL_LockSurface(surface);
    SDL_Rect rect;
    rect.x = rx;
    rect.y = ry;
    rect.w = rw;
    rect.h = rh;
    SDL_FillRect(surface, &rect, SDL_MapRGBA(surface->format, r, g, b, 255));
    SDL_UnlockSurface(surface);
    // SDL_UpdateWindowSurface(window);
    // usleep(w * h / 1000);
}

void _setPixel(int x, int y, int color) {
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (x > 959)
        x = 959;
    if (y > 240 * 3 - 1)
        y = 240 * 3 - 1;
    int r = (color >> 11) * 255 / 31;
    int g = ((color >> 5) % 64) * 255 / 63;
    int b = (color % 32) * 255 / 31;
    SDL_LockSurface(surface);
    *((Uint32*)surface->pixels + y * 320 * 3 + x) = SDL_MapRGBA(surface->format, r, g, b, 255);
    SDL_UnlockSurface(surface);
}

void tftPixel(int x, int y, int c) {
    _setPixel(HR - y - 1, x, c);
}

void tftVline(int x, int y, int w, int c) {
    for (int i = 0; i < w; i++)
        _setPixel(HR - y - i - 1, x, c);
}

void tfHline(int x, int y, int l, int c) {
    for (int i = 0; i < l; i++)
        _setPixel(HR - y - 1, x + i, c);
}

// TOUCH SCREEN

int listenToTouchesOnScreen() {
    return 0;
}

int           px = 0;
int           py = 0;
unsigned char mouseDown = 0;
unsigned char mouseUp = 0;

int touched() {
    listenToTouchesOnScreen();
    int result = mouseUp;
    if (mouseUp) {
        mouseUp = 0;
        mouseDown = 0;
    }
    return result;
}

int getTouchX() {
    return px;
}

int getTouchY() {
    return py;
}

void setTouchX(int x) {
    px = x;
}

void setTouchY(int y) {
    py = y;
}

void setMouseUp(int up) {
    mouseUp = up;
}

// CONSOLE

void console(const char* c) {
    printf("%s", c);
}

void console(float f) {
    printf("%f", f);
}

void console(int i) {
    printf("%d", i);
}

void consoleln(const char* c) {
    printf("%s\n", c);
}

void consoleln(unsigned long c) {
    printf("%lu\n", c);
}

void consoleln(float f) {
    printf("%f\n", f);
}

void consoleln(int i) {
    printf("%d\n", i);
}

void consoleln() {
    printf("\n");
}

// STATIC READING

void useStaticStrings() {
}

void useDynamicStrings() {
}

char pgm_read_byte(const char* a) {
    return *a;
}

const int pgm_read_word_near(const int* a) {
    return *a;
}

char pgm_read_byte_near(const unsigned char* a) {
    return *a;
}

const char* PSTR(const char* a) {
    return a;
}

// FINISHING

void terminate() {
    exit(1);
}

// RAM

int heap[10000];
int _eprom[10000];

void writeToHeap(int a, unsigned char c) {
    heap[a] = c;
}

// EPROM

void writeByteToEEPROM(int address, unsigned char value) {
    if (readByteFromEEPROM(address) == value) {
        return;
    }
    if (address < 0 || address >= EPROM_SIZE)
        fatalError(17, address);
    _eprom[address] = value;
}

void writeIntToEEPROM(int address, unsigned int value) {
    writeByteToEEPROM(address, value / 256);
    writeByteToEEPROM(address + 1, value % 256);
}

unsigned char readByteFromEEPROM(int address) {
    if (address < 0 || address >= EPROM_SIZE)
        fatalError(18, address);
    return (unsigned char)_eprom[address];
}

unsigned int readIntFromEEPROM(int address) {
    return readByteFromEEPROM(address) * 256 + readByteFromEEPROM(address + 1);
}

int random(int r) {
    return rand() % r;
}
#endif
