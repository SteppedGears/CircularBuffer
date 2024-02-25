/***************
 * CircularBuffer.h
 * 
*/

#include <stdint.h>

typedef struct circularBuffer_s{
    uint8_t *buf;
    uint8_t *pStart;
    uint8_t *pEnd;
    uint8_t *pWrite;
    uint8_t *pRead;
    uint8_t *pMark; 
}circularBuffer_t;

void CircularBufferInit(circularBuffer_t *pCircularBuffer, uint8_t *pBuf, size_t bufSize);
size_t CircularBufferFreeSpace(circularBuffer_t *pBuffer);
int CircularBufferIsEmpty(circularBuffer_t *pBuffer);
int CircularBufferWriteByte(circularBuffer_t *pBuffer, uint8_t byte);
uint8_t CircularBufferReadByte(circularBuffer_t *pBuffer);
