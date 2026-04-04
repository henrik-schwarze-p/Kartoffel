//
//  Launcher.c
//  Kartoffel
//
//  Created by Enrico on 20.09.20.
//  Copyright © 2020 K. Schwarze. All rights reserved.
//

#include "Launcher.h"
#include "BoardLib.h"
#include "Instance.h"
#include "Target.h"
#include "Text.h"
#include "Drawing.h"
#include "MainLoop.h"
#include "Heap.h"
#include "Chunks.h"
#include "Descriptors.h"
#include "Registration.h"
#include "Arduino.h"
#include "Rules.h"

#include "EepromConf.inc.h"
#include "FatalError.h"

unsigned char readConf(int a) {
    return pgm_read_byte_near(eepromConfiguration + a);
}

void launchAll() {
    boardPrintln("Launching...");

    unsigned char epromRandom0 = readByteFromEEPROM(EPROM_RANDOM);
    unsigned char epromRandom1 = readByteFromEEPROM(EPROM_RANDOM + 1);

    // Case 0: Random Bits match -> not first booting
    if (randomBits[0] == epromRandom0 && randomBits[1] == epromRandom1) {
        boardPrintln("Not first booting");
        for (int i = 0; i < MAX_NUMBER_OF_INSTANCES; i++)
            if (usedInstance(i))
                launchInstance(i);
        return;
    }

    // Case 1: Configured in appStore
    // => Configuration + random bits written to EPROM
    if (readConf(0) != 0xFE) {
        boardPrintln("There is a configuration.");
        boardPrintln("Configuration is going to be written to the EPROM.");
        setPrintY(20);
        drawCenteredString(PSTR("Configuring..."));
        for (int i = 0; i < EPROM_SIZE; i++) {
            if (i % 64 == 0)
                fillRect(i / 64 * 5, 100, 5, 2);
            writeByteToEEPROM(i, readConf(i));
        }
        writeByteToEEPROM(EPROM_RANDOM, randomBits[0]);
        writeByteToEEPROM(EPROM_RANDOM + 1, randomBits[1]);
        boardPrintln("Configuration written.");

        // Clock is configured

        for (int i = 0; i < MAX_NUMBER_OF_INSTANCES; i++)
            if (usedInstance(i))
                launchInstance(i);
        return;
    }

    // Case 2
    // No configuration, random bits dont match
    boardPrintln("Random bits don't match, writing data for the first time.");
    boardPrintln("Date: 2026/03/05 13:30:05");

    setClockYear(26);
    setClockMonth(3);
    setClockDay(5);
    setClockHours(13);
    setClockMins(30);
    setClockSecs(5);

    writeByteToEEPROM(EPROM_FORMATTED, (uint8_t)45);
    writeByteToEEPROM(EPROM_FORMATTED + 1, (uint8_t)41);

    writeByteToEEPROM(PERSISTANT_HEAP_ADDRESS + P_INSTANCE, UNUSED_CHUNK);
    writeByteToEEPROM(PERSISTANT_HEAP_ADDRESS + P_HANDLE, 0);
    writeIntToEEPROM(PERSISTANT_HEAP_ADDRESS + P_SIZE, (unsigned int)(EPROM_SIZE - PERSISTANT_HEAP_ADDRESS - P_DATA));

    writeByteToEEPROM(EPROM_RANDOM, randomBits[0]);
    writeByteToEEPROM(EPROM_RANDOM + 1, randomBits[1]);

    boardPrintln("Adding one instance of each app.");

    prepareInstanceTable();
    for (int i = 0; i < NUMBER_OF_DESCRIPTORS; i++)
        launchDescriptor(i);
}

int launchDescriptor(int descriptor) {
    // Where are we going to write in the table?

    int newInstance = -1;

    // search for the first hole
    for (int i = 0; i < MAX_NUMBER_OF_INSTANCES; i++) {
        if (statusForInstance(i, STATUS_UNUSED)) {
            newInstance = i;
            break;
        }
    }

    if (newInstance == -1) {
        fatalError(511, 0);
    }

    // First thing: the instance id
    setIdForInstance(newInstance, descriptor);

    // Then the status: OK, not registering
    // From here, it is already safe to change contexts (because of registering)
    writeInstanceStatus(newInstance, STATUS_OK);

    boardPrint(PSTR("Launching "));
    boardPrintln(nameForInstance(newInstance));

    // First, we call when created, the persistant variable will be initialized
    // But they are still not registered, so context switching is harmless
    callWhenCreated(newInstance);

    // Persistant variables will be registered, and they will begin to switch.
    // Instances status is changed in this fn.
    switchContextToInstance(newInstance);

    registerAllocPersistantMemory();
    serializeRegisteredVariables();
    popContext();

    launchInstance(newInstance);
    return newInstance;
}

void launchInstance(int instance) {
    // The instance was already created, it should be in the table
    // If it crashed because of no memory, we clear the bit
    removeStatusForInstance(instance, STATUS_NO_MEMORY);
    removeStatusForInstance(instance, STATUS_REGISTERM_ACTIVATED);

    // It opens starts screen by default
    setEnterScreen(0, instance);

    // Memory variables get initialized
    callWhenPowered(instance);

    // We will alloc memory for the registered variables, and context
    // switching will start for them.
    switchContextToInstance(instance);
    registerAllocMemory();
    popContext();
}
