#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "CircularBuffer.h"

#define CIRCULAR_BUFFER_SIZE 100

#define NUM_BYTES 100

static void *writingThread(void *arg);
static void *readingThread(void *arg);

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

    pthread_create(&threads[1], NULL, readingThread, &circularBuffer);
    pthread_create(&threads[0], NULL, writingThread, &circularBuffer);
    

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    return 0;
}

static void *writingThread(void *arg){
    circularBuffer_t *pBuffer = (circularBuffer_t *)arg;
    int i;
    uint8_t byteToWrite;

    for(i = 0; i < NUM_BYTES; i++){
        byteToWrite = i%256;
        CircularBufferWriteByte(pBuffer, byteToWrite);
        //printf("write byte: 0x%02X\n", byteToWrite);
        usleep(1000);
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
        }
        usleep(1000);
    }
    printf("last read byte: %02X\n", byte);
    return 0;
}