#ifndef KernelChunks_h
#define KernelChunks_h

#include <stdint.h>
#include <unistd.h>

#define MAIN_CHUNK_HANDLE 1
#define EPROM_SIZE 4096
#define P_INSTANCE 0
#define P_HANDLE 1
#define P_SIZE 2
#define P_DATA 4

#define PERSISTANT_HEAP_ADDRESS 130

#define UNUSED_CHUNK 255
#define TMP_HANDLE 254

int firstChunkAddress();
int nextChunkAddress(int);

void    freeAllChunksForActiveInstance();
uint8_t chunkDataGetValue(int chunk, int i);
uint8_t chunkInstance(int);
uint8_t chunkHandle(int);
int     chunkUnused(int);
int     chunkDataSize(int);

unsigned int dataSizeForHandle(int handle);
void         mergeUnusedChunks();

int  handleExists(int handle);
int  handleExists(int instance, int handle);
int  chunkDataAddressForInstanceAndHandle(int instance, int handle);
void  resizeChunk(unsigned int handle, int newSize);
void freeChunk(int handle);
int chunkAddressForHandle(int handle);
unsigned int availableDataMemory();

void          setDataMain(int address, unsigned char value);
unsigned char readData(int handle, int address);
unsigned char readMainData(int address);
void          setData(int handle, int address, unsigned char value);
void          dumpChunks();
void  allocChunk(int handle, int size);
void           insertDataSegment(int handle, int address, int delta);
void           deleteDataSegment(int handle, int address, int delta);
unsigned int  availableDataMemory();
int           readDataInt(int address);
void          setDataInt(int address, unsigned int value);
void          setDataMainFloat(int address, float value);
float         readMainDataFloat(int address);

#endif
