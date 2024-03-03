#include <stdio.h>
#include "CircularBuffer.h"

static void incrementRead(circularBuffer_t *pBuffer);

void CircularBufferInit(circularBuffer_t *pCircularBuffer, uint8_t *pBuf, size_t bufSize){
    pCircularBuffer->pStart = pBuf;
    pCircularBuffer->pEnd = pBuf + bufSize -1;
    pCircularBuffer->pWrite = pCircularBuffer->pStart;
    pCircularBuffer->pRead = pCircularBuffer->pStart;
    pCircularBuffer->pMark = pCircularBuffer->pStart;
}

size_t CircularBufferFreeSpace(circularBuffer_t *pBuffer){
    return 99;
}

int CircularBufferIsEmpty(circularBuffer_t *pBuffer){
    if(pBuffer->pRead == pBuffer->pWrite){
        return 1;
    }
    return 0;
}

int CircularBufferWriteByte(circularBuffer_t *pBuffer, uint8_t byte){
    *(pBuffer->pWrite) = byte;
    pBuffer->pWrite++;
    if(pBuffer->pWrite > pBuffer->pEnd){
        pBuffer->pWrite = pBuffer->pStart;
    }
    if(pBuffer->pWrite == pBuffer->pRead){
        incrementRead(pBuffer);
    }
}

uint8_t CircularBufferReadByte(circularBuffer_t *pBuffer){
    uint8_t byte;
    byte = *(pBuffer->pRead);
    incrementRead(pBuffer);
    return byte;
}

static void incrementRead(circularBuffer_t *pBuffer){
    pBuffer->pRead++;
    if(pBuffer->pRead > pBuffer->pEnd){
        pBuffer->pRead = pBuffer->pStart;
    }
}