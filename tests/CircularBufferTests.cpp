#include "CppUTest/TestHarness.h"



extern "C"
{
	#include "CircularBuffer.h"
}

TEST_GROUP(CircularBufferBasicInit)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(CircularBufferBasicInit, newBufferCreated)
{
    const ssize_t bufferSize = 100;
    uint8_t buffer[bufferSize];
    circularBuffer_t circularBuffer;

    CircularBufferInit(&circularBuffer, buffer, bufferSize);
    POINTERS_EQUAL(buffer, circularBuffer.pStart);
    POINTERS_EQUAL(buffer + bufferSize - 1, circularBuffer.pEnd);
    POINTERS_EQUAL(buffer, circularBuffer.pRead);
    POINTERS_EQUAL(buffer, circularBuffer.pWrite);
    POINTERS_EQUAL(buffer, circularBuffer.pMark);
    
}

TEST(CircularBufferBasicInit, newBufferIsEmpty)
{
    const ssize_t bufferSize = 100;
    uint8_t buffer[bufferSize];
    circularBuffer_t circularBuffer;

    CircularBufferInit(&circularBuffer, buffer, bufferSize);
    CHECK_EQUAL(1, CircularBufferIsEmpty(&circularBuffer));
}

TEST_GROUP(CircularBufferBasic)
{
    static const ssize_t bufferSize = 10;
    uint8_t buffer[bufferSize];
    circularBuffer_t circularBuffer;
    void setup()
    {
        memset(buffer, 0xAA, bufferSize);
        CircularBufferInit(&circularBuffer, buffer, bufferSize);
    }

    void teardown()
    {
    }
};

TEST(CircularBufferBasic, writtenBufferIsNotEmpty)
{
    CircularBufferWriteByte(&circularBuffer, 'A');
    CHECK_EQUAL(0, CircularBufferIsEmpty(&circularBuffer));
}

TEST(CircularBufferBasic, writtenBufferContainsWrittenByte)
{
    CircularBufferWriteByte(&circularBuffer, 'A');
    CHECK_EQUAL('A', buffer[0]);
}

TEST(CircularBufferBasic, writtenBufferContainsOnlyWrittenByte)
{
    uint8_t expectedBuffer[bufferSize];
    memset(expectedBuffer, 0xAA, bufferSize);
    expectedBuffer[0] = 'A';

    CircularBufferWriteByte(&circularBuffer, 'A');
    MEMCMP_EQUAL(expectedBuffer, buffer, bufferSize);
}

TEST(CircularBufferBasic, canReadWrittenByte)
{
    
    CircularBufferWriteByte(&circularBuffer, 'A');
    CHECK_EQUAL('A', CircularBufferReadByte(&circularBuffer));
}

TEST(CircularBufferBasic, canWriteTwoBytes)
{
    uint8_t expectedBuffer[bufferSize];
    memset(expectedBuffer, 0xAA, bufferSize);
    expectedBuffer[0] = 'A';
    expectedBuffer[1] = 'B';
    CircularBufferWriteByte(&circularBuffer, 'A');
    CircularBufferWriteByte(&circularBuffer, 'B');
    MEMCMP_EQUAL(expectedBuffer, buffer, bufferSize);
}

TEST(CircularBufferBasic, canReadTwoWrittenByte)
{
    
    CircularBufferWriteByte(&circularBuffer, 'A');
    CircularBufferWriteByte(&circularBuffer, 'B');
    BYTES_EQUAL('A', CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('B', CircularBufferReadByte(&circularBuffer));
}