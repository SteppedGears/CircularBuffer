#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "CircularBuffer.h"

#define CIRCULAR_BUFFER_SIZE 100

#define NUM_BYTES 1000

typedef struct{
    circularBuffer_t *pBuffer;
    int numWrites;
}writeThreadArgs_t;

static void *writingThread(void *arg);
static void *readingThread(void *arg);
static void *readingThreadWithRewind(void *arg);

static int stillWriting = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER ;

int main(void){
    
    printf("Multi thread tests\n");

    uint8_t buffer[CIRCULAR_BUFFER_SIZE];
    circularBuffer_t circularBuffer;
    CircularBufferInit(&circularBuffer, buffer, CIRCULAR_BUFFER_SIZE);

    pthread_t threads[2];
    pthread_mutex_lock(&mutex);
    stillWriting = 1;
    pthread_mutex_unlock(&mutex);

    writeThreadArgs_t writeThreadArgs;
    writeThreadArgs.pBuffer = &circularBuffer;
    writeThreadArgs.numWrites = NUM_BYTES;

    pthread_create(&threads[1], NULL, readingThread, &circularBuffer);
    pthread_create(&threads[0], NULL, writingThread, &writeThreadArgs);
    

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    pthread_mutex_lock(&mutex);
    stillWriting = 1;
    pthread_mutex_unlock(&mutex);
    writeThreadArgs.numWrites = 3000;
    pthread_create(&threads[1], NULL, readingThreadWithRewind, &circularBuffer);
    pthread_create(&threads[0], NULL, writingThread, &writeThreadArgs);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    return 0;
}

static void *writingThread(void *arg){
    writeThreadArgs_t *pArgs = (writeThreadArgs_t *)arg;
    circularBuffer_t *pBuffer = pArgs->pBuffer;
    int numWrites = pArgs->numWrites;
    int i;
    uint8_t byteToWrite;

    for(i = 0; i < numWrites; i++){
        byteToWrite = i%256;
        CircularBufferWriteByte(pBuffer, byteToWrite);
        //printf("write byte: 0x%02X\n", byteToWrite);
        usleep(100);
    }
    printf("last written byte: %02X\n", byteToWrite);
    pthread_mutex_lock(&mutex);
    stillWriting = 0;
    pthread_mutex_unlock(&mutex);
    return 0;
}

static void *readingThread(void *arg){
    circularBuffer_t *pBuffer = (circularBuffer_t *)arg;
    uint8_t byte;
    int stillWritingTmp;

    while(1){
        pthread_mutex_lock(&mutex);
        stillWritingTmp = stillWriting;
        pthread_mutex_unlock(&mutex);
        
        if(!CircularBufferIsEmpty(pBuffer)){
            byte = CircularBufferReadByte(pBuffer);
            //printf("Read byte: 0x%02X\n", byte);
        }else if (!stillWritingTmp){
            break;
        }else{
            usleep(1000);
        }
    }
    printf("last read byte: %02X\n", byte);
    return 0;
}

static void *readingThreadWithRewind(void *arg){
    circularBuffer_t *pBuffer = (circularBuffer_t *)arg;
    uint8_t byte;
    int stillWritingTmp;
    int rewound = 0;
    int marked = 0;

    while(1){
        pthread_mutex_lock(&mutex);
        stillWritingTmp = stillWriting;
        pthread_mutex_unlock(&mutex);
        
        if(!CircularBufferIsEmpty(pBuffer)){
            byte = CircularBufferReadByte(pBuffer);
            if(byte == 0x0A && !marked){
                CircularBufferSetMarker(pBuffer);
                marked = 1;
            }else if(byte == 0x10 && !rewound){
                CircularBufferRewind(pBuffer);
                rewound = 1;
            }
            printf("Read byte: 0x%02X\n", byte);
        }else if (!stillWritingTmp){
            break;
        }else{
            usleep(1000);
        }
        
    }
    printf("last read byte: %02X\n", byte);
    return 0;
}