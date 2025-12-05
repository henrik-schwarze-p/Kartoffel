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

void printOffset(int offset) {
    boardPrint("0x");
    boardPrint(offset, 4);
    boardPrint(":      ");
}

void printEE(int offset) {
    boardPrint(readByteFromEEPROM(offset), 2);
    boardPrint(" ");
}

void col2() {
    boardPrint("    ");
}

void dumpChunks() {
    boardPrintln("EEPROM contents");
    int offset = 0;
    printOffset(offset);
    printEE(offset++);
    boardPrint(readByteFromEEPROM(offset++), 2);
    col2();

    boardPrintln("Magic number");
    printOffset(offset);
    int numberOfInstances = readByteFromEEPROM(offset);
    printEE(offset++);
    col2();

    boardPrintln("Number of instances");
    int i = 0;
    for (i = 0; i < numberOfInstances; i++) {
        printOffset(offset);
        printEE(offset++);
        printEE(offset++);
        printEE(offset++);
        col2();
        boardPrint(instanceName(i));
        boardPrint(" ");
        if (statusForInstance(i, STATUS_OK))
            boardPrint("OK ");
        else
            boardPrint("ERROR ");
        if (statusForInstance(i, STATUS_REGISTERM_ACTIVATED))
            boardPrint("REGM ");
        if (statusForInstance(i, STATUS_REGISTERP_ACTIVATED))
            boardPrint("REGP ");
        boardPrintln("");
    }
    for (; i < MAX_NUMBER_OF_INSTANCES; i++) {
        printOffset(offset);
        printEE(offset++);
        printEE(offset++);
        printEE(offset++);
        col2();

        boardPrintln("Unused");
    }
    printOffset(offset);
    printEE(offset++);
    printEE(offset++);
    col2();

    boardPrintln("Random numbers to check EEPROM unserialisation");
    printOffset(offset);
    while (offset < 105) {
        printEE(offset++);
    }
    col2();

    boardPrintln("Reserved");
    printOffset(offset);
    while (offset < 115) {
        printEE(offset++);
    }
    col2();

    boardPrintln("Reserved");
    printOffset(offset);
    while (offset < 123) {
        printEE(offset++);
    }
    col2();
    boardPrintln("Reserved");
    printOffset(offset);
    printEE(offset++);
    printEE(offset++);
    printEE(offset++);
    printEE(offset++);
    printEE(offset++);
    col2();
    boardPrintln("Serialized Datetime (Y-M-D H:M)");
    printOffset(offset);
    while (offset < 130) {
        printEE(offset++);
    }
    col2();
    boardPrintln("Unused");
    int _chunk = goToFirstChunk();
    while (!endOfChunks(_chunk)) {
        printOffset(_chunk);
        printEE(_chunk);
        printEE(_chunk + 1);
        printEE(_chunk + 2);
        printEE(_chunk + 3);
        col2();
        int isRules = 0;
        if (chunkHandle(_chunk) == LOG_HANDLE) {
            boardPrint("Log Handle ");
        } else if (chunkHandle(_chunk) == RULES_HANDLE) {
            isRules = 1;
            boardPrint("Rules Handle ");
        } else if (chunkHandle(_chunk) == NAME_HANDLE)
            boardPrint("Name Handle ");
        else if (chunkHandle(_chunk) == MAIN_CHUNK_HANDLE)
            boardPrint("Main Handle ");
        else {
            boardPrint("(");
            boardPrint(chunkHandle(_chunk));
            boardPrint(")");
        }
        boardPrint("allocated by ");
        boardPrint(instanceName(chunkInstance(_chunk)));
        boardPrint("/Handle/Size");
        boardPrintln("");

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
            boardPrint("Next offset should be 0x1000 ");
        } else {
            printOffset(_chunk + 4);
            int max = 0;
            int l = nextChunk(_chunk) - _chunk;
            for (int i = P_OVERHEAD; i < l; i++) {
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
        _chunk = nextChunk(_chunk);
    }
    printOffset(_chunk);
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
