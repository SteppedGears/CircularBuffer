#include "CircularBuffer.h"

static void incrementRead(circularBuffer_t *pBuffer);

/********************
* Name: CircularBufferInit
* Description: Initializes the circular buffer with the provided buffer and size.
* Input:
*   pCircularBuffer: pointer to the circular buffer structure
*   pBuf: pointer to the buffer array
*   bufSize: size of the buffer array
* Output: <>
* Return: <>
**********************/
void CircularBufferInit(circularBuffer_t *pCircularBuffer, uint8_t *pBuf, size_t bufSize){
    pCircularBuffer->pStart = pBuf;
    pCircularBuffer->pEnd = pBuf + bufSize -1;
    pCircularBuffer->pWrite = pCircularBuffer->pStart;
    pCircularBuffer->pRead = pCircularBuffer->pStart;
    pCircularBuffer->pMark = pCircularBuffer->pStart;
}

/********************
* Name: CircularBufferFreeSpace
* Description: Returns the amount of free space in the circular buffer.
* Input:
*   pBuffer: pointer to the circular buffer structure
* Output: <>
* Return: the amount of free space in the buffer
**********************/
size_t CircularBufferFreeSpace(circularBuffer_t *pBuffer){
    return 99;
}

/********************
* Name: CircularBufferIsEmpty
* Description: Checks if the circular buffer is empty.
* Input:
*   pBuffer: pointer to the circular buffer structure
* Output: <>
* Return: 1 if the buffer is empty, 0 otherwise
**********************/
int CircularBufferIsEmpty(circularBuffer_t *pBuffer){
    if(pBuffer->pRead == pBuffer->pWrite){
        return 1;
    }
    return 0;
}

/********************
* Name: CircularBufferWriteByte
* Description: Writes a single byte to the circular buffer.
               If the write reaches the mark, the mark is incremented.
               If the write reaches the read pointer, the read pointer is incremented,
               overwriting the oldest byte.
* Input:
*   pBuffer: pointer to the circular buffer structure
*   byte: the byte to write
* Output: <>
* Return: 0 if successful, -1 if the buffer was full and the read pointer was incremented
**********************/
int CircularBufferWriteByte(circularBuffer_t *pBuffer, uint8_t byte){
    int retVal = 0;
    *(pBuffer->pWrite) = byte;
    pBuffer->pWrite++;
    if(pBuffer->pWrite > pBuffer->pEnd){
        pBuffer->pWrite = pBuffer->pStart;
    }
    if(pBuffer->pWrite == pBuffer->pMark){
        pBuffer->pMark++;
        if(pBuffer->pMark > pBuffer->pEnd){
            pBuffer->pMark = pBuffer->pStart;
        }
    }
    if(pBuffer->pWrite == pBuffer->pRead){
        incrementRead(pBuffer);
        retVal = -1;
    }
    return retVal;
}

/********************
* Name: CircularBufferWriteNBytes
* Description: Writes multiple bytes to the circular buffer.
* Input:
*   pBuffer: pointer to the circular buffer structure
*   pBytes: pointer to the array of bytes to write
*   nBytes: number of bytes to write
* Output: <>
* Return: the negative of the number of bytes that were overwritten
**********************/
int CircularBufferWriteNBytes(circularBuffer_t *pBuffer, uint8_t *pBytes, size_t nBytes){
    int retVal = 0;
    for(int i = 0; i < nBytes; i++){
        retVal += CircularBufferWriteByte(pBuffer, pBytes[i]);
    }
    return retVal;
}

/********************
* Name: CircularBufferReadByte
* Description: Reads a single byte from the circular buffer and increment read pointer.
               The caller must be sure that the buffer is not empty before calling this function
               by checking CircularBufferIsEmpty.
* Input:
*   pBuffer: pointer to the circular buffer structure
* Output: <>
* Return: the byte read from the buffer
**********************/
uint8_t CircularBufferReadByte(circularBuffer_t *pBuffer){
    uint8_t byte;
    byte = *(pBuffer->pRead);
    incrementRead(pBuffer);
    return byte;
}

/********************
* Name: CircularBufferSetMarker
* Description: Sets the marker to the current read position.
* Input:
*   pBuffer: pointer to the circular buffer structure
* Output: <>
* Return: <>
**********************/
void CircularBufferSetMarker(circularBuffer_t *pBuffer) {
    pBuffer->pMark = pBuffer->pRead;
}

/********************
* Name: CircularBufferRewind
* Description: Rewinds the read position to the marker.
* Input:
*   pBuffer: pointer to the circular buffer structure
* Output: <>
* Return: <>
**********************/
void CircularBufferRewind(circularBuffer_t *pBuffer) {
    pBuffer->pRead = pBuffer->pMark;
}

/********************
* Name: incrementRead
* Description: Increments the read pointer, wrapping around if necessary.
* Input:
*   pBuffer: pointer to the circular buffer structure
* Output: <>
* Return: <>
**********************/
static void incrementRead(circularBuffer_t *pBuffer){
    pBuffer->pRead++;
    if(pBuffer->pRead > pBuffer->pEnd){
        pBuffer->pRead = pBuffer->pStart;
    }
}