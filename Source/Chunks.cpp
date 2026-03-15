#include "Chunks.h"
#include "Instance.h"
#include "FatalError.h"
#include "Target.h"

//
// CHUNK MANAGEMENT
// Chunks are memory allocations in the eprom.
// Each chunk has an identifier GIVEN BY THE USER.
//
/* The persistant memory heap consiste of many chunks.

 A chunk has the following structure

 0           INSTANCE (FF==unused)
 1           HANDLE
 2,3         SIZE (the empty chunk length may use 2 bytes...)
 4           BEGIN OF DATA
 4+SIZE-1    END OF DATA

 4+SIZE      NEXT CHUNK

 That is, we have 4 bytes overhead per chunk.

 Chunks use always the whole memory available. That means that the last chunk
 has a datasize big enough that its last byte is at the end of the EEPROM.
 */

// ALL THE FUNCTIONS WORK FOR THE ACTIVE INSTANCE

// All chunks have an absoute address, that is used by most functions.
// I denote <chunkAddress> as the chunk starting at address chunkAddress.



/*
 * To iterate over chunks
 */

// Address of first chunk in memory
int firstChunkAddress() {
    return PERSISTANT_HEAP_ADDRESS;
}

// the address of the next chunk relative to <chunkAddress>
int nextChunkAddress(int chunkAddress) {
    int next= chunkAddress + chunkDataSize(chunkAddress) + P_DATA;
    if (next==EPROM_SIZE) {
        return 0;
    }
    return next;
}

// the instance of <chunkAddress>, FF means unused chunk
uint8_t chunkInstance(int chunkAddress) {
    return readByteFromEEPROM(chunkAddress + P_INSTANCE);
}

// the handle of <chunkAddress>. Handles are given by the user!
uint8_t chunkHandle(int chunkAddress) {
    return readByteFromEEPROM(chunkAddress + P_HANDLE);
}

// no instance is using this <chunkAddress>?
int chunkUnused(int chunkAddress) {
    return readByteFromEEPROM(chunkAddress + P_INSTANCE) == UNUSED_CHUNK;
}

// how big is the data portion of the <chunkAddress>?
// it is not the size of the <chunkAddress>!
int chunkDataSize(int chunkAddress) {
    return readIntFromEEPROM(chunkAddress + P_SIZE);
}


/*
* Free chunks for an instance
*/

// Frees all chunks for the active instance and merge the unused
void freeAllChunksForActiveInstance() {
    int chunkAddress = firstChunkAddress();
    while (chunkAddress) {
        if (chunkInstance(chunkAddress) == getActiveInstance())
            writeByteToEEPROM(chunkAddress + P_INSTANCE, UNUSED_CHUNK);
        chunkAddress = nextChunkAddress(chunkAddress);
    }
    mergeUnusedChunks();
}



/*
* From here on, we only use handles
* Handle is used as an abbreviation for "chunk with the given hundle"
*/

// chunk data size for a given handle for the active instance
unsigned int dataSizeForHandle(int handle) {
    int chunkAddress = chunkAddressForHandle(handle);
    return readIntFromEEPROM(chunkAddress + P_SIZE);
}

// is there a chunk for the given instance and handle?
int handleExists(int instance, int handle) {
    int chunkAddress = firstChunkAddress();
    while (chunkAddress) {
        if (chunkHandle(chunkAddress) == handle && chunkInstance(chunkAddress) == instance)
            return 1;
        chunkAddress = nextChunkAddress(chunkAddress);
    }
    return 0;
}

// is there a chunk for the given handle and active instance?
int handleExists(int handle) {
    return handleExists(getActiveInstance(), handle);
}

// the chunk address for the handle of the active instance
int chunkAddressForHandle(int handle) {
    int chunkAddress = firstChunkAddress();
    while (chunkAddress) {
        if (chunkHandle(chunkAddress) == handle && chunkInstance(chunkAddress) == getActiveInstance())
            return chunkAddress;
        chunkAddress = nextChunkAddress(chunkAddress);
    }
    fatalError(31, handle);
    return 0;
}

// Returns the address of the data, not the beginning of the chunk
// try to use the accessors instead, use only when you need a mem address!
int chunkDataAddressForInstanceAndHandle(int instance, int handle) {
    int chunkAddress = firstChunkAddress();
    while (chunkAddress) {
        if (chunkHandle(chunkAddress) == handle && chunkInstance(chunkAddress) == instance)
            return chunkAddress + P_DATA;
        chunkAddress = nextChunkAddress(chunkAddress);
    }
    fatalError(32, handle);
    return 0;
}

// how much data space is there (it is an estimation, the real size may be a few bytes bigger)
unsigned int availableDataMemory() {
    int r = 0;
    int chunkAddress = firstChunkAddress();
    while (chunkAddress) {
        if (chunkUnused(chunkAddress))
            r += chunkDataSize(chunkAddress);
        chunkAddress = nextChunkAddress(chunkAddress);
    }
    return r;
}

// tries to alloc a handle for a given data size for the active instance
// throws fatal error if no space
void allocChunk(int handle, int dataSize) {
    // find an unused chunk bigger than size
    int chunkAddress = firstChunkAddress();
    int recycledChunkDataSize = 0;
    while (chunkAddress) {
        recycledChunkDataSize = chunkDataSize(chunkAddress);
        if (chunkUnused(chunkAddress) && dataSize + 2 * P_DATA <= recycledChunkDataSize)  // overhead + overhead for empty chunk
            break;
        chunkAddress = nextChunkAddress(chunkAddress);
    }
    if (!chunkAddress) {
        addStatusForInstance(STATUS_NO_MEMORY);
        fatalError(33, handle);
    }
    // we break the unused chunk to be recycled into a used an unused one
    writeByteToEEPROM(chunkAddress, getActiveInstance());
    writeByteToEEPROM(chunkAddress + P_HANDLE, handle);
    writeIntToEEPROM(chunkAddress + P_SIZE, dataSize);

    int u = chunkAddress + P_DATA + dataSize;
    writeByteToEEPROM(u, UNUSED_CHUNK);
    writeByteToEEPROM(u + P_HANDLE, (unsigned int)0);
    writeIntToEEPROM(u + P_SIZE, (unsigned int)(recycledChunkDataSize - dataSize - P_DATA));

    // handy sometimes, we initialize the data
    if (dataSize)
        setData(handle, 0, 0);
}


// Merges contiguous unused chunks.
// Not very efficient from the point of view of the algorithm, but it is more
// simple AND the part that spends the time is anyway modifying the EEPROM, not
// reading it.
void mergeUnusedChunks() {
    int atLeastOneMerged = 1;
    while (atLeastOneMerged) {
        atLeastOneMerged = 0;
        int lastChunkAddress = 0;
        int chunkAddress = firstChunkAddress();
        while (chunkAddress) {
            if (lastChunkAddress && chunkUnused(lastChunkAddress) && chunkUnused(chunkAddress)) {
                writeIntToEEPROM(lastChunkAddress + P_SIZE,
                                 chunkDataSize(lastChunkAddress) + chunkDataSize(chunkAddress) + P_DATA);
                atLeastOneMerged = 1;
                break;
            }
            lastChunkAddress = chunkAddress;
            chunkAddress = nextChunkAddress(chunkAddress);
        }
    }
}

// frees a chunk for the given handle for the active instance
void freeChunk(int handle) {
    int chunkAddress = chunkAddressForHandle(handle);
    writeByteToEEPROM(chunkAddress + P_INSTANCE, UNUSED_CHUNK);
    writeByteToEEPROM(chunkAddress + P_HANDLE, (unsigned int)0);
    mergeUnusedChunks();
}

// resizes a chunk, not used for the time being
// throws error if not enough space
void resizeChunk(unsigned int handle, int newSize) {
    allocChunk(TMP_HANDLE, newSize);

    int existingChunkAddress = chunkAddressForHandle(handle);
    int newChunkAddress = chunkAddressForHandle(TMP_HANDLE);

    // copy data
    for (int i = 0; i < newSize; i++)
        writeByteToEEPROM(newChunkAddress + P_DATA + i, readByteFromEEPROM(existingChunkAddress + P_DATA + i));
    
    // old handle markes as unused
    freeChunk(handle);

    writeByteToEEPROM(newChunkAddress + P_HANDLE, handle);
}



// DATA ACCESSORS
// We only expose the handles of the active instance as arrays, no chunk addresses are exposed
// Throw fatal if out of bounds

unsigned char readData(int handle, int offset) {
    int chunkAddress = chunkAddressForHandle(handle);
    if (offset < 0 || offset >= chunkDataSize(chunkAddress))
        fatalError(34, offset);
    return readByteFromEEPROM(chunkAddress + P_DATA + offset);
}

unsigned char readMainData(int offset) {
    return readData(MAIN_CHUNK_HANDLE, offset);
}

int readDataInt(int offset) {
    return readData(MAIN_CHUNK_HANDLE, offset) * 256 + readData(MAIN_CHUNK_HANDLE, offset + 1);
}

uint32_t readDataLong(int offset) {
    uint32_t       result;
    unsigned char* bytes = (unsigned char*)&result;
    bytes[0] = readMainData(offset);
    bytes[1] = readMainData(offset + 1);
    bytes[2] = readMainData(offset + 2);
    bytes[3] = readMainData(offset + 3);
    return result;
}

float readDataFloat(int handle, int offset) {
    float          result = 0.0;
    unsigned char* bytes = (unsigned char*)&result;
    bytes[0] = readMainData(offset);
    bytes[1] = readMainData(offset + 1);
    bytes[2] = readMainData(offset + 2);
    bytes[3] = readMainData(offset + 3);
    return result;
}

float readMainDataFloat(int offset) {
    return readDataFloat(MAIN_CHUNK_HANDLE, offset);
}

void setData(int handle, int offset, unsigned char value) {
    int chunkAddress = chunkAddressForHandle(handle);
    if (offset < 0 || offset >= chunkDataSize(chunkAddress))
        fatalError(35, offset);
    if (readByteFromEEPROM(chunkAddress + P_DATA + offset) != value) {
        writeByteToEEPROM(chunkAddress + P_DATA + offset, value);
    }
}

void setDataMain(int offset, unsigned char value) {
    setData(MAIN_CHUNK_HANDLE, offset, value);
}

void setDataInt(int offset, unsigned int value) {
    setData(MAIN_CHUNK_HANDLE, offset, value / 256);
    setData(MAIN_CHUNK_HANDLE, offset + 1, value % 256);
}

void setDataFloat(int handle, int offset, float value) {
    unsigned char* bytes = (unsigned char*)&value;
    setData(handle, offset, bytes[0]);
    setData(handle, offset + 1, bytes[1]);
    setData(handle, offset + 2, bytes[2]);
    setData(handle, offset + 3, bytes[3]);
}

void setDataMainFloat(int offset, float value) {
    setDataFloat(MAIN_CHUNK_HANDLE, offset, value);
}

// deletes a segment inside the given chunk
// the segment starts at offset, and consists of lenght bytes
void deleteDataSegment(int handle, int offset, int length) {
    int chunkAddress = chunkAddressForHandle(handle);
    int size =  chunkDataSize(chunkAddress);
    int newSize = size - length;

    allocChunk(TMP_HANDLE, size - length);

    int tmpChunkAddress = chunkAddressForHandle(TMP_HANDLE);
    for (int i = 0; i < newSize; i++) {
        if (i < offset)
            writeByteToEEPROM(tmpChunkAddress + P_DATA + i, readByteFromEEPROM(chunkAddress + P_DATA + i));
        else
            writeByteToEEPROM(tmpChunkAddress + P_DATA + i, readByteFromEEPROM(chunkAddress + P_DATA + i + length));
    }

    freeChunk(handle);
    
    writeByteToEEPROM(tmpChunkAddress + P_HANDLE, handle);
}

// inserts a segment in the data of the given chunk
void insertDataSegment(int handle, int offset, int length) {
    // create a temporary chunk (that is going to be the result)
    int chunkAddress = chunkAddressForHandle(handle);
    int size = chunkDataSize(chunkAddress);
    allocChunk(TMP_HANDLE, size + length);

    // offset=5, length=3
    // 0123456
    // 01234---56

    int tmpChunkAddress = chunkAddressForHandle(TMP_HANDLE);
    for (int i = 0; i < size; i++) {
        if (i < offset)
            writeByteToEEPROM(tmpChunkAddress + P_DATA + i, readByteFromEEPROM(chunkAddress + P_DATA + i));
        else
            writeByteToEEPROM(tmpChunkAddress + P_DATA + i + length, readByteFromEEPROM(chunkAddress + P_DATA + i));
    }

    freeChunk(handle);

    writeByteToEEPROM(tmpChunkAddress + P_HANDLE, handle);
}