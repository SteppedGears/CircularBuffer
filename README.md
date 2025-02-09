# CircularBuffer with rewind
Circular buffer is a simple circular buffer (ring buffer) library implemented in C. It supports multi-threaded environments with thread-safe operations using mutexes.

As it uses a user provided buffer as the underlying storage, it works well with both dynamic and static buffers.
Having the capability to work with static buffers, allows it to be used in embedded projects where dynamic memory allocation can be problematic.

## Features

- Initialization of circular buffer with a specified size
- Writing single or multiple bytes to the buffer
- Reading bytes from the buffer
- Checking if the buffer is empty
- Setting and rewinding to a marker position
- Thread-safe operations using pthread mutexes (on Linux)
- In case of full buffer, adding a new byte will delete the oldest one

## How to build
This project uses CMake so the usual commands can be used:
```bash
mkdir build
cd build
cmake ../
cmake --build .
```

### Notes
- Tests use CppUTest, so you may want to clone the submodule
- Multi-thread test is done with Valgrind, so you need to have it on your system
- Testing works on Linux (WSL)

## Usage

### Initialization
To initialize the circular buffer, use the `CircularBufferInit()` function:

```C
#include "CircularBuffer.h"

#define BUFFER_SIZE 100

uint8_t buffer[BUFFER_SIZE];
circularBuffer_t circularBuffer;

CircularBufferInit(&circularBuffer, buffer, BUFFER_SIZE);
```
**Note:** The circular buffer will contain 1 byte less than the size of the underlying buffer.

### Writing to the buffer

To write a single byte to the buffer, use the `CircularBufferWriteByte()` function:
```C
uint8_t byte = 0x01;
CircularBufferWriteByte(&circularBuffer, byte);
```

To write multiple bytes to the buffer, use the `CircularBufferWriteNBytes()` function:
```C
uint8_t bytes[] = {0x01, 0x02, 0x03};
CircularBufferWriteNBytes(&circularBuffer, bytes, sizeof(bytes));
```

### Reading from the buffer
**Note:** Before reading from the buffer, be sure that there is something to read by calling `CircularBufferIsEmpty()`.


To read a single byte from the buffer, use the `CircularBufferReadByte()` function:
```C
if(!CircularBufferIsEmpty(&circularBuffer)){
    uint8_t byte = CircularBufferReadByte(&circularBuffer);
}
```

### Setting and Rewinding to a marker
Especially when looking for a string in a buffer, it may be useful to be able to rewind
to the last valid position to wait for it to be completed. For this there is the marker
and rewind feature.
So set a marker at the current read position, use the `CircularBufferSetMarker()`:
```C
CircularBufferSetMarker(&circularBuffer);
```
To rewind the read position to the marker, use the `CircularBufferRewind()` function:
```C
CircularBufferRewind(&circularBuffer);
```
**Note:** The mark pointer will be moved as well, if the write pointer reaches it.

## Multi-threading

A possible use for this buffer is having a writing thread and reading thread.
For example, in an embedded system, the bytes are written to the buffer from the
ISR, and then read and parsed from the main context. In this way, the ISR can be
as short as possible.

