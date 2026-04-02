#include "Debug.h"
#include "BoardLib.h"
#include "Heap.h"
#include "Instance.h"
#include "Target.h"
#include "Chunks.h"
#include "Log.h"
#include "Rules.h"
#include "Text.h"
#include "Target.h"

#include <stdio.h>

extern unsigned char heapData[HEAP_SIZE];

#ifdef MEGA
void printOffset(int offset) {
}

void printEE(int offset) {
}

void col2() {
}

void dumpChunks() {
}

void dumpHeaps() {
}

void dumpHeapsTxt() {
}
#else
#include <cstring>

void printOffset(int offset) {
    boardPrint("0x");
    boardPrint(offset, 4);
    boardPrint(":      ");
}

void printEE(int offset) {
    boardPrint(readByteFromEEPROM(offset), 2);
    boardPrint(" ");
}

int kmin(int a, int b) {
    if (a < b)
        return a;
    return b;
}

void col2() {
    boardPrint("    ");
}

char chacha[1000];
char chacha2[1000];
int  koffset;

void kprint(const char* s) {
    strcat(chacha, s);
}

int  ll = 5;
void kprint(int kb) {
    int b = kb;
    if (b > 50) {
        b = 50;
    }
    printOffset(koffset);
    for (int i = 0; i < kmin(ll, b); i++)
        printEE(koffset + i);

    for (int i = 0; i < ll - kmin(ll, b); i++)
        boardPrint("   ");

    boardPrint("   ");
    boardPrintln(chacha);
    strcpy(chacha, "");

    for (int i = ll; i < b; i++) {
        if (i % ll == 0) {
            printOffset(koffset + i);
            printEE(koffset + i);
        }
        if (i % ll > 0 && i % ll < ll - 1) {
            printEE(koffset + i);
        }
        if (i % ll == ll - 1) {
            printEE(koffset + i);
            boardPrintln("");
        }
    }
    if (b > ll and (b % ll) != 0) {
        boardPrintln();
    }

    koffset += kb;
}

void kprintn(int n) {
    snprintf(chacha2, 10, "%d", n);
    kprint(chacha2);
}

void dumpChunks() {
    strcpy(chacha, "");
    koffset = 0;

    int numberOfInstances = readByteFromEEPROM(2);

    boardPrintln("EEPROM contents");

    kprint("Magic number");
    kprint(2);
    kprint("Number of instancess");
    kprint(1);

    for (int i = 0; i < numberOfInstances; i++) {
        if (!statusForInstance(i, STATUS_UNUSED)) {
            kprint(instanceName(i));
            kprint(" ");
        }
        if (statusForInstance(i, STATUS_UNUSED))
            kprint("UNUSED ");
        if (statusForInstance(i, STATUS_REGISTERM_ACTIVATED))
            kprint("REGM ");
        if (statusForInstance(i, STATUS_REGISTERP_ACTIVATED))
            kprint("REGP ");
        if (statusForInstance(i, STATUS_OK))
            kprint("OK ");
        kprint(3);
    }

    kprint("Random numbers to check EEPROM unserialisation");
    kprint(2);

    kprint("Reserved");
    kprint(123 - koffset);

    kprint("Serialized Datetime (Y-M-D H:M)");
    kprint(5);

    kprint("Unused");
    kprint(130 - koffset);

    int _chunk = firstChunkAddress();
    while (_chunk) {
        int isRules = 0;
        if (chunkHandle(_chunk) == LOG_HANDLE) {
            kprint("Log Handle ");
        } else if (chunkHandle(_chunk) == RULES_HANDLE) {
            isRules = 1;
            kprint("Rules Handle ");
        } else if (chunkHandle(_chunk) == NAME_HANDLE)
            kprint("Name Handle ");
        else if (chunkHandle(_chunk) == MAIN_CHUNK_HANDLE)
            kprint("Main Handle ");
        else {
            kprint("Handle ");
            kprintn(chunkHandle(_chunk));
            kprint(": ");
        }

        kprint("allocated by ");
        kprint(instanceName(chunkInstance(_chunk)));
        kprint(4);

        kprint("Data");
        kprint(chunkDataSize(_chunk));
        _chunk = nextChunkAddress(_chunk);
    }
    printOffset(koffset);
    boardPrintln();

    /*
            if (isRules) {
                int pop = 4;
                printOffset(_chunk + pop);
                printEE(_chunk + pop);
                col2();
                boardPrintln("Active condition");
                pop++;

                while (readByteFromEEPROM(_chunk + pop) != 0xED) {
                    printOffset(_chunk + pop);
                    printEE(_chunk + pop);
                    col2();
                    int isC = readByteFromEEPROM(_chunk + pop) == 0xC0;
                    if (isC)
                        boardPrintln("It is a condition");
                    else
                        boardPrintln("It is an action");
                    pop++;

                    printOffset(_chunk + pop);
                    printEE(_chunk + pop);
                    printEE(_chunk + pop + 1);
                    col2();
                    boardPrint("Instance ");
                    int instance = readByteFromEEPROM(_chunk + pop);
                    int whichOne = readByteFromEEPROM(_chunk + pop + 1);
                    boardPrint(instanceName(instance));
                    boardPrint("/");
                    boardPrintln("Offset of condition or action ");
                    pop += 2;

                    int         expectedNumberOfParameters = 0;
                    const char* c;
                    if (isC) {
                        c = callConditionNames(instance, whichOne);
                    } else {
                        c = callActionNames(instance, whichOne);
                    }
                    int i = 0;
                    while (pgm_read_byte(c + i)) {
                        if (pgm_read_byte(c + i) == '[')
                            expectedNumberOfParameters++;
                        i++;
                    }
                    printOffset(_chunk + pop);
                    for (int i = 0; i < expectedNumberOfParameters * 2; i++) {
                        printEE(_chunk + pop);
                        pop++;
                    }
                    col2();
                    boardPrintln("Parameters");
                }
                printOffset(_chunk + pop);
                printEE(_chunk + pop);
                col2();
                boardPrintln("End of rules");
            } else if (chunkInstance(_chunk) == 255) {
                printOffset(_chunk + 4);
                col2();
            } else {
                printOffset(_chunk + 4);
                int max = 0;
                int l = nextChunkAddress(_chunk) - _chunk;
                for (int i = P_DATA; i < l; i++) {
                    if (max == 8) {
                        max = 0;
                        boardPrintln("");
                        printOffset(_chunk + i);
                    }
                    printEE(_chunk + i);
                    max++;
                }
                col2();
            }
            boardPrintln("");
            _chunk = nextChunkAddress(_chunk);
        }
        printOffset(_chunk);*/
}

void dumpHeaps() {
    int head = 0;
    boardPrintln("HEAPS");
    while (heapData[head + H_INSTANCE] != UNUSED_HEAP) {
        boardPrint("   ");
        boardPrint(head);
        boardPrint("   ");
        boardPrint(instanceName(heapData[head + H_INSTANCE]));
        boardPrint(" Handle:");
        if (heapData[head + H_HANDLE] == 249)
            boardPrint("TIMERS");
        else if (heapData[head + H_HANDLE] == 1)
            boardPrint("MAIN");
        else
            boardPrint(heapData[head + H_HANDLE]);
        boardPrint(" Len:");
        int len = heapData[head + H_LEN];
        boardPrint(len);
        boardPrint(" [");
        if (len > 20)
            len = 20;
        for (int i = 0; i < len; i++) {
            boardPrint(heapData[head + H_LEN + 1 + i]);
            boardPrint(" ");
        }
        boardPrintln("]");
        head += H_OVERHEAD + heapData[head + H_LEN];
    }
}

void dumpHeapsTxt() {
    int head = 0;
    console("HEAP");
    consoleln();
    while (heapData[head + H_INSTANCE] != UNUSED_HEAP) {
        console(head);
        console(" Instance:");
        console(instanceName(heapData[head + H_INSTANCE]));
        console(" Handle:");
        console(heapData[head + H_HANDLE]);
        console(" Len:");
        console(heapData[head + H_LEN]);
        console(" [");

        for (int i = 0; i < heapData[head + H_LEN]; i++) {
            console(heapData[head + H_DATA + i]);
            console(" ");
        }
        console("]");
        consoleln();
        head += H_OVERHEAD + heapData[head + H_LEN];
    }
}
#endif
