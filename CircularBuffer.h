/***************
 * CircularBuffer.h
 * 
*/

#include <stdint.h>
#include <stddef.h>

#ifdef __linux__
#include <pthread.h>
#endif

typedef struct circularBuffer_s{
    uint8_t *buf;
    uint8_t *pStart;
    uint8_t *pEnd;
    uint8_t *pWrite;
    uint8_t *pRead;
    uint8_t *pMark;
    #ifdef __linux__
    pthread_mutex_t mutex;
    #endif
}circularBuffer_t;

void CircularBufferInit(circularBuffer_t *pCircularBuffer, uint8_t *pBuf, size_t bufSize);
size_t CircularBufferFreeSpace(circularBuffer_t *pBuffer);
int CircularBufferIsEmpty(circularBuffer_t *pBuffer);
int CircularBufferWriteByte(circularBuffer_t *pBuffer, uint8_t byte);
int CircularBufferWriteNBytes(circularBuffer_t *pBuffer, uint8_t *pBytes, size_t nBytes);
uint8_t CircularBufferReadByte(circularBuffer_t *pBuffer);
void CircularBufferSetMarker(circularBuffer_t *pBuffer);
void CircularBufferRewind(circularBuffer_t *pBuffer);
