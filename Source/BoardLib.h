//
//  BoardLib.h
//  Kartoffel
//
//  Created by Kurt Schwarze on 11.02.19.
//  Copyright © 2019 Kurt Schwarze. All rights reserved.
//

#ifndef BoardLib_h
#define BoardLib_h

#ifdef MEGA
void boardPrintln(const char* c);
void boardPrint(const char* c);
void initBoard();
void boardRepaint();

#else

void initBoard();
int  boardReadAnalog(int);
int  boardReadDigital(int p);
void boardPrintln(const char* c);
void boardPrintln(int u);
void boardPrint(const char* c);
void boardPrint(int u);
void boardPrint(float);
void boardTextRepaint();
void boardPrintln();

void boardRepaint();

void boardDigitalPortClicked(int param);
void boardAnalogPortClicked(int param);
void boardAnalogValueClicked(int param);
void boardPrint(int number, int digits);

void boardWriteDigital(int p, int v);
void boardWriteAnalog(int p, int v);

#endif
#endif /* BoardLib_h */
