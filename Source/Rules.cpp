#include "Target.h"
#include "Chunks.h"
#include "FatalError.h"
#include "Rules.h"
#include "Descriptors.h"
#include "Heap.h"
#include "Instance.h"
#include "Namespaces.h"

/*
 RULES STRUCTURE
 Offset   Element
 0        Active Combined Condition Index (index, not address!, of the first
          condition of the combined condition. ff=none). For example, for
          CC,AC,CC,CC,AC,CC*,CC,AC the index is 5
 1        Conditions+
 ?        Actions*
 ?        Conditions+
 ?        Actions*
 ?        0xED End

 Or simplified

 Active
 Axon of index 0
 Axon of index 1
 ...
 EndAxon (uses only 1 byte)

 AXON STRUCTURE
 C0 (HEAD)  Condition (C0) | Action (A0) | End (0)
 03         Defined in instance 3
 05         Kind #5, defined in instance
 03 07         Parameter #0 (Number of parameters defined in instance
 02 01         Parameter #1

 The length of the Axon is NOT stored in the structure.

 An Axon can be an Action, a Condition, NOT_FOUND or End.
 */

const char* axonLabel(Axon a);

// CONSTRUCTORS

Axon notFoundAxon() {
    Axon a;
    a.notFound = 1;
    return a;
}

/*
 Creates an axon looking at the bytes starting @ axonAddress.
 Important: the struct does not corresponde 1:1 to the bytes.
 memcpy(address, axon) does not work!
 */
Axon axonForAddress(int axonAddress) {
    // what is the first byte of the axon
    int head = pget(RULES_HANDLE, axonAddress);

    Axon a;
    a.isEnd = head == AX_END;
    a.isAction = head == AX_IS_ACTION;
    a.isCondition = head == AX_IS_CONDITION;
    a.address = axonAddress;
    a.notFound = 0;

    // the end axon has just an address and the "isEnd" set
    if (a.isEnd)
        return a;

    a.headValue = head;
    a.address = axonAddress;
    a.instance = pget(RULES_HANDLE, axonAddress + AX_INSTANCE_OFFSET);
    a.kind = pget(RULES_HANDLE, axonAddress + AX_KIND_OFFSET);

    a.numberOfParams = numberOfParams(a);
    a.label = axonLabel(a);

    for (int i = 0; i < a.numberOfParams; i++) {
        int index = axonAddress + AX_PARAMS_OFFSET + i * sizeof(uint16_t);
        // we have to work under different architectures
        a.params[i] = pget(RULES_HANDLE, index) * 256 + pget(RULES_HANDLE, index + 1);
    }
    a.length = AX_PARAMS_OFFSET + a.numberOfParams * sizeof(uint16_t);

    a.nextAddress = axonAddress + a.length;

    return a;
}

/*
 A label for the axon to show it on screen.
 */
const char* axonLabel(Axon a) {
    if (a.isEnd)
        fatalError(1, 0);
    if (a.isCondition)
        return callConditionNames(a.instance, a.kind);
    return callActionNames(a.instance, a.kind);
}

/*
 The number of paramteres is part of the name of the Axon.
 Never more than 5.
 */
int numberOfParams(Axon a) {
    const char* c = axonLabel(a);
    int         expectedNumberOfParameters = 0;
    int         i = 0;
    while (pgm_read_byte(c + i)) {
        if (pgm_read_byte(c + i) == '[')
            expectedNumberOfParameters++;
        i++;
    }
    if (expectedNumberOfParameters > 5)
        fatalError(44, a.instance);
    return expectedNumberOfParameters;
}

Axon insertAxon(int index, int isCondition, int instance, int kind) {
    Axon a;
    a.headValue = AX_IS_ACTION;
    if (isCondition)
        a.headValue = AX_IS_CONDITION;
    a.instance = instance;
    a.kind = kind;
    a.isEnd = 0;
    a.notFound = 0;
    a.isAction = !isCondition;
    a.isCondition = isCondition;
    a.numberOfParams = numberOfParams(a);
    a.label = axonLabel(a);
    for (int i = 0; i < a.numberOfParams; i++)
        a.params[i] = 0;
    a.length = AX_PARAMS_OFFSET + a.numberOfParams * sizeof(uint16_t);

    Axon existingAxon = axonForIndex(index);
    a.address = existingAxon.address;
    insertChunkFragment(RULES_HANDLE, existingAxon.address, a.length);
    writeAxon(a);
    return a;
}

InstanceKind instanceAndKindForGlobalIndex(int considerConditions, int globalAxonIndex) {
    InstanceKind ik;
    const char* (*fn)(int, int) = callActionNames;
    if (considerConditions)
        fn = callConditionNames;
    int acc = 0;
    for (int instance = 0; instance < numberOfInstances(); instance++) {
        int         index = 0;
        const char* r = fn(instance, index);
        while (r) {
            if (acc == globalAxonIndex) {
                ik.instance = instance;
                ik.kind = index;
                ik.found = 1;
                return ik;
            }
            acc++;
            index++;
            r = fn(instance, index);
        }
    }
    ik.found = 0;
    return ik;
}

Axon nextAxon(Axon a) {
    if (a.isEnd)
        fatalError(2, 2);
    return axonForAddress(a.nextAddress);
}

Axon axonForIndex(int axonIndex) {
    Axon axon = axonForAddress(AX_FIRST_AXON_ADDRESS);
    while (axonIndex > 0) {
        axon = nextAxon(axon);
        axonIndex--;
    }
    return axon;
}

// WRITE AXON

void writeAxon(Axon a) {
    int writeAddress = a.address;
    pset(RULES_HANDLE, writeAddress++, a.headValue);
    if (a.isEnd)
        return;
    pset(RULES_HANDLE, writeAddress++, a.instance);
    pset(RULES_HANDLE, writeAddress++, a.kind);
    for (int i = 0; i < a.numberOfParams; i++) {
        pset(RULES_HANDLE, writeAddress++, a.params[i] / 256);
        pset(RULES_HANDLE, writeAddress++, a.params[i] % 256);
    }
}

int indexOfAxon(Axon a) {
    Axon axon = axonForIndex(0);
    int  i = 0;
    while (!axon.isEnd && a.address != axon.address) {
        axon = nextAxon(axon);
        i++;
    }
    if (!axon.isEnd)
        return i;
    return -1;
}

int numberOfAxons() {
    Axon a = axonForIndex(0);
    int  n = 0;
    while (!a.isEnd) {
        a = nextAxon(a);
        n++;
    }
    return n;
}

// Insert and delete

void insertEmptyAction(int axonIndex) {
    insertAxon(axonIndex, 0, instanceForId(portManagerId()), 0);
}

void insertEmptyCondition(int axonIndex) {
    insertAxon(axonIndex, 1, instanceForId(portManagerId()), 0);
}

void insertAction(int axonIndex, int globalAxonIndex) {
    InstanceKind ik = instanceAndKindForGlobalIndex(0, globalAxonIndex);
    insertAxon(axonIndex, 0, ik.instance, ik.kind);
}

void insertCondition(int axonIndex, int globalAxonIndex) {
    InstanceKind ik = instanceAndKindForGlobalIndex(1, globalAxonIndex);
    insertAxon(axonIndex, 1, ik.instance, ik.kind);
}

void deleteAxon(int axonIndex) {
    Axon axon = axonForIndex(axonIndex);
    deleteChunkFragment(RULES_HANDLE, axon.address, axon.length);
}

// LABELS

int equals(const char* text, const char* pstr) {
    int i = 0;
    while (pgm_read_byte(pstr + i)) {
        if (pgm_read_byte(pstr + i) != pgm_read_byte(text + i))
            return 0;
        i++;
    }
    return pgm_read_byte(text + i) == ']';
}

// "hi [A] and [B]", index == 0, compareTo=="A" returns true
// "hi [A] and [B]", index == 1, compareTo=="B" returns true
int toolbarIntForParameterX(const char* text, int index, const char* compareTo) {
    int i = 0;
    while (pgm_read_byte(text + i)) {
        if (pgm_read_byte(text + i) == '[') {
            i++;
            if (index == 0)
                return equals(text + i, compareTo);
            index--;
        }
        i++;
    }
    return 0;
}

int toolbarIntForParameter(const char* text, int index, const char* toWhat) {
    int i = 0;
    while (pgm_read_byte(text + i)) {
        if (pgm_read_byte(text + i) == '[') {
            i++;
            if (index == 0)
                return equals(text + i, toWhat);
            index--;
        }
        i++;
    }
    fatalError(4, index);
    return 1;
}

int toolbarLabelForParameter1(const char* text, int index) {
    int i = 0;
    while (pgm_read_byte(text + i)) {
        if (pgm_read_byte(text + i) == '[') {
            i++;
            if (index == 0) {
                return i;
            }
            index--;
        }
        i++;
    }
    fatalError(74, index);
    return 0;
}

int toolbarLabelForParameter2(const char* text, int index) {
    int i = 0;
    while (pgm_read_byte(text + i)) {
        if (pgm_read_byte(text + i) == ']') {
            i++;
            if (index == 0) {
                return i - 2;
            }
            index--;
        }
        i++;
    }
    return 0;
}

int thereIsAnActiveCC() {
    return pget(RULES_HANDLE, AX_ACTIVE) != 255;
}

Axon activeCC() {
    int active = pget(RULES_HANDLE, AX_ACTIVE);
    if (active == 255)
        return notFoundAxon();
    return axonForIndex(active);
}

int indexOfActiveCC() {
    return pget(RULES_HANDLE, AX_ACTIVE);
}

void setActiveCC(Axon cc) {
    pset(RULES_HANDLE, AX_ACTIVE, indexOfAxon(cc));
}

// KKK

int evalCC(Axon cc) {
    // The only way a conditions stops, is when another one is true
    if (activeCC().address == cc.address)
        return 0;
    Axon a = cc;
    while (a.isCondition) {
        if (!callEvalCondition(a.instance, a.kind, a.params))
            return 0;
        a = nextAxon(a);
    }
    // The CC is true
    setActiveCC(cc);
    while (a.isAction) {
        callPerformAction(a.instance, a.kind, a.params);
        a = nextAxon(a);
    }
    return 1;
}

Axon firstCC() {
    Axon a = axonForIndex(0);
    while (a.isAction)
        a = nextAxon(a);
    return a;
}

Axon nextCC(Axon cc) {
    while (cc.isCondition)
        cc = nextAxon(cc);
    if (cc.isEnd)
        return cc;
    while (cc.isAction)
        cc = nextAxon(cc);
    return cc;
}

int evalRules() {
    int changed = 0;
    for (int inst = 0; inst < numberOfInstances(); inst++) {
        if (chunkForInstanceAndHandleExists(inst, RULES_HANDLE)) {
            switchContextToInstance(inst);
            for (Axon cc = firstCC(); !cc.isEnd; cc = nextCC(cc)) {
                changed += evalCC(cc);
            }
            popContext();
        }
    }
    return changed;
}

void instanceWasRemoved(int instance) {
    for (int i = 0; i < numberOfAxons(); i++) {
        Axon axon = axonForIndex(i);
        if (axon.instance >= instance) {
            axon.instance--;
            writeAxon(axon);
        }
    }
}

int rulesReference(int instance) {
    for (int i = 0; i < numberOfAxons(); i++)
        if (axonForIndex(i).instance == instance)
            return 1;
    return 0;
}

/**
 Is there any instance that have rules that reference this instance?
 */
int rulesUsingInstance(int instance) {
    int result = 0;
    int _chunk = goToFirstChunk();
    while (!endOfChunks(_chunk)) {
        if (chunkHandle(_chunk) == RULES_HANDLE && chunkInstance(_chunk) != UNUSED_CHUNK &&
            chunkInstance(_chunk) != instance) {
            switchContextToInstance(chunkInstance(_chunk));
            result += rulesReference(instance);
            popContext();
        }
        _chunk = nextChunk(_chunk);
    }
    return result;
}

/**
 The instance was removed. Indexes refering to other instances must be updated.
 */
void updateRulesBecauseOfDeletionOfInstance(int instance) {
    int _chunk = goToFirstChunk();
    while (!endOfChunks(_chunk)) {
        if (chunkHandle(_chunk) == RULES_HANDLE && chunkInstance(_chunk) != UNUSED_CHUNK) {
            switchContextToInstance(chunkInstance(_chunk));
            instanceWasRemoved(instance);
            popContext();
        }
        _chunk = nextChunk(_chunk);
    }
}

void createRulesChunk() {
    allocChunk(RULES_HANDLE, 2);
    pset(RULES_HANDLE, 0, 255);
    pset(RULES_HANDLE, AX_FIRST_AXON_ADDRESS, AX_END);
}

// TESTS

#ifdef MEGA
#else
#include "BoardLib.h"
#include <stdio.h>
#include <string.h>
#endif

int axonDumpEq(const char* c) {
    char* r = (char*)malloc(1000);
    char* s = (char*)malloc(100);
    strcpy(r, "");
    for (int i = 0; i < 1000; i++) {
        sprintf(s, "%02x", pget(RULES_HANDLE, i));
        strcat(r, s);
        strcat(r, " ");
        if (pget(RULES_HANDLE, i) == 0xED) {
            int eq = !strcmp(c, r);
            if (!eq) {
                boardPrintln("COMPARISION FAILED");
                boardPrint("Expected: ");
                boardPrintln(c);
                boardPrint("    Real: ");
                boardPrintln(r);
            }
            return eq;
        }
    }
    return 0;
}

void testRules() {
    int diceInstance = 9;
    int pinManager = 14;
    int admin=1;
    int tds=18;
    int ph=13;
#ifdef MEGA
#else
    boardPrintln("===================================");
    boardPrintln("Testing rules system...");
    
    createRulesChunk();
    boardPrint("* Creates chunk, there should be 0 axons: ");
    boardPrintln(numberOfAxons() == 0);

    boardPrint("* Insert workds: ");
    boardPrintln(axonDumpEq("ff ed "));
    
    // the first axon adress is 1, 0 is reserved for the index of the active condition
    Axon a = axonForAddress(AX_FIRST_AXON_ADDRESS);
    boardPrint("* First axon is end: ");
    boardPrintln(a.isEnd);

    insertAxon(0, 1, diceInstance, 0);
    boardPrint("* Insert dice: ");
    boardPrintln(axonDumpEq("ff c0 09 00 00 00 ed "));

    insertAxon(0, 0, pinManager, 0);
    boardPrint("* Insert pin: ");
    boardPrintln(axonDumpEq("ff a0 0e 00 c0 09 00 00 00 ed "));

    insertAxon(1, 1, tds, 1);
    boardPrint("* Insert tds: ");
    boardPrintln(axonDumpEq("ff a0 0e 00 c0 12 01 00 00 c0 09 00 00 00 ed "));

    a = axonForAddress(1);
    boardPrint("* Axon 0: ");
    boardPrint(a.kind == 0);
    boardPrintln(a.numberOfParams==0);

    a = axonForAddress(4);
    boardPrint("* Axon 1: ");
    boardPrint(a.kind == 1);
    boardPrintln(a.numberOfParams==1);

    a = axonForAddress(9);
    boardPrint("* Axon 2: ");
    boardPrint(a.kind == 0);
    boardPrintln(a.numberOfParams==1);

    a = axonForAddress(1);
    boardPrint("* Next axon 0: ");
    boardPrint(a.kind == 0);
    boardPrintln(a.numberOfParams==0);

    a = nextAxon(a);
    boardPrint("* Next axon 1: ");
    boardPrint(a.kind == 1);
    boardPrintln(a.numberOfParams==1);

    a = nextAxon(a);
    boardPrint("* Next axon 2: ");
    boardPrint(a.kind == 0);
    boardPrintln(a.numberOfParams==1);

    a=axonForIndex(0);
    boardPrint("* Axon for index 0: ");
    boardPrintln(a.address==1);

    a=axonForIndex(1);
    boardPrint("* Axon for index 1: ");
    boardPrintln(a.address==4);

    a=axonForIndex(2);
    boardPrint("* Axon for index 2: ");
    boardPrintln(a.address==9);

    
    deleteAxon(1);
    boardPrint("* Delete axon 1: ");
    boardPrintln(axonDumpEq("ff a0 0e 00 c0 09 00 00 00 ed "));

    deleteAxon(1);
    boardPrint("* Delete axon 1: ");
    boardPrintln(axonDumpEq("ff a0 0e 00 ed "));

    deleteAxon(0);
    boardPrint("* Delete axon 0: ");
    boardPrintln(axonDumpEq("ff ed "));

    insertAxon(0, 1, diceInstance, 0);
    insertAxon(0, 0, pinManager, 0);
    insertAxon(1, 1, tds, 1);
    boardPrint("* Reinsert 3 axons: ");
    boardPrintln(axonDumpEq("ff a0 0e 00 c0 12 01 00 00 c0 09 00 00 00 ed "));

    deleteAxon(0);
    boardPrint("* Delete axon 0: ");
    boardPrintln(axonDumpEq("ff c0 12 01 00 00 c0 09 00 00 00 ed "));

    a=axonForIndex(1);
    a.params[0]=3;
    writeAxon(a);
    boardPrint("* Write axon: ");
    boardPrintln(axonDumpEq("ff c0 12 01 00 00 c0 09 00 00 03 ed "));

    boardPrint("* Number of axons: ");
    boardPrintln(numberOfAxons()==2);

    setActiveCC(a);
    boardPrint("* Active CC set: ");
    boardPrintln(activeCC().address==6);

#endif
}
