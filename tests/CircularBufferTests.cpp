#include "CppUTest/TestHarness.h"
#include "CppUTest/UtestMacros.h"
#include <cstdint>



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
    static const ssize_t realBufferSize = bufferSize + 1;
    uint8_t buffer[realBufferSize];
    circularBuffer_t circularBuffer;
    void setup()
    {
        memset(buffer, 0xAA, realBufferSize);
        CircularBufferInit(&circularBuffer, buffer, bufferSize);
    }

    void teardown()
    {
        CHECK_EQUAL(0xAA, buffer[realBufferSize - 1]);
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
    uint8_t expectedBuffer[realBufferSize];
    memset(expectedBuffer, 0xAA, realBufferSize);
    expectedBuffer[0] = 'A';

    CircularBufferWriteByte(&circularBuffer, 'A');
    MEMCMP_EQUAL(expectedBuffer, buffer, realBufferSize);
}

TEST(CircularBufferBasic, canReadWrittenByte)
{
    
    CircularBufferWriteByte(&circularBuffer, 'A');
    CHECK_EQUAL('A', CircularBufferReadByte(&circularBuffer));
}

TEST(CircularBufferBasic, canWriteTwoBytes)
{
    uint8_t expectedBuffer[realBufferSize];
    memset(expectedBuffer, 0xAA, realBufferSize);
    expectedBuffer[0] = 'A';
    expectedBuffer[1] = 'B';
    CircularBufferWriteByte(&circularBuffer, 'A');
    CircularBufferWriteByte(&circularBuffer, 'B');
    MEMCMP_EQUAL(expectedBuffer, buffer, realBufferSize);
}

TEST(CircularBufferBasic, canReadTwoWrittenByte)
{
    
    CircularBufferWriteByte(&circularBuffer, 'A');
    CircularBufferWriteByte(&circularBuffer, 'B');
    BYTES_EQUAL('A', CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('B', CircularBufferReadByte(&circularBuffer));
}

TEST(CircularBufferBasic, bufferIsEmptyAfterReadingAll)
{
    CircularBufferWriteByte(&circularBuffer, 'A');
    CircularBufferWriteByte(&circularBuffer, 'B');
    CircularBufferReadByte(&circularBuffer);
    CircularBufferReadByte(&circularBuffer);
    CHECK_EQUAL(1, CircularBufferIsEmpty(&circularBuffer));
}

TEST(CircularBufferBasic, bufferWriteIsCircular)
{
    uint8_t expectedBuffer[realBufferSize];
    memset(expectedBuffer, 0xAA, realBufferSize);
    int i;
    for( i = 0; i < bufferSize; i++){
        expectedBuffer[i] = '0' + i;
    }
    expectedBuffer[0] = '0' + i;

    for(int i = 0; i < realBufferSize; i++){
        CircularBufferWriteByte(&circularBuffer, '0' + i);
    }

    MEMCMP_EQUAL(expectedBuffer, buffer, realBufferSize);
}

TEST(CircularBufferBasic, fullBufferIsNotEmpty)
{
    for(int i = 0; i < bufferSize; i++){
        CircularBufferWriteByte(&circularBuffer, '0' + i);
    }
    CHECK_EQUAL(0, CircularBufferIsEmpty(&circularBuffer));
}

TEST(CircularBufferBasic, bufferReadIsCircular)
{
    for(int i = 0; i < realBufferSize; i++){
        CircularBufferWriteByte(&circularBuffer, '0' + i);
    }

    BYTES_EQUAL('0' + 2, CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('0' + 3, CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('0' + 4, CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('0' + 5, CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('0' + 6, CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('0' + 7, CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('0' + 8, CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('0' + 9, CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('0' + 10, CircularBufferReadByte(&circularBuffer));
}

TEST(CircularBufferBasic, canOverwriteMoreThanOnce)
{
    uint8_t expectedBuffer[realBufferSize];
    expectedBuffer[realBufferSize - 1] = 0xAA;
    int i;
    for( i = 0; i < (realBufferSize + bufferSize); i++){
        expectedBuffer[i % bufferSize] = '0' + i;
    }

    for(int i = 0; i < (realBufferSize + bufferSize); i++){
        CircularBufferWriteByte(&circularBuffer, '0' + i);
    }
    MEMCMP_EQUAL(expectedBuffer, buffer, realBufferSize);
}

TEST(CircularBufferBasic, canReadAfterOverwrite)
{
    int i;

    for(i = 0; i < (realBufferSize + bufferSize); i++){
        CircularBufferWriteByte(&circularBuffer, '0' + i);
    }

    BYTES_EQUAL(0x3C, CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL(0x3D, CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL(0x3E, CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL(0x3F, CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL(0x40, CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL(0x41, CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL(0x42, CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL(0x43, CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL(0x44, CircularBufferReadByte(&circularBuffer));
}

TEST(CircularBufferBasic, canSetMarkerAndRewindAndReread)
{
    CircularBufferWriteByte(&circularBuffer, 'A');
    CircularBufferWriteByte(&circularBuffer, 'B');
    CircularBufferWriteByte(&circularBuffer, 'C');
    CircularBufferSetMarker(&circularBuffer);
    BYTES_EQUAL('A', CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('B', CircularBufferReadByte(&circularBuffer));
    CircularBufferRewind(&circularBuffer);
    BYTES_EQUAL('A', CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('B', CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('C', CircularBufferReadByte(&circularBuffer));
}

TEST(CircularBufferBasic, markerIsMovedWhenReachedByWrite)
{
    CircularBufferWriteByte(&circularBuffer, 'A');
    CircularBufferWriteByte(&circularBuffer, 'B');
    CircularBufferWriteByte(&circularBuffer, 'C');

    BYTES_EQUAL('A', CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('B', CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('C', CircularBufferReadByte(&circularBuffer));
    CircularBufferSetMarker(&circularBuffer);

    CircularBufferWriteByte(&circularBuffer, 'D');
    CircularBufferWriteByte(&circularBuffer, 'E');
    CircularBufferWriteByte(&circularBuffer, 'F');
    CircularBufferWriteByte(&circularBuffer, 'G');
    CircularBufferWriteByte(&circularBuffer, 'H');
    CircularBufferWriteByte(&circularBuffer, 'I');
    CircularBufferWriteByte(&circularBuffer, 'J');

    BYTES_EQUAL('D', CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('E', CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('F', CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('G', CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('H', CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('I', CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('J', CircularBufferReadByte(&circularBuffer));



    CircularBufferWriteByte(&circularBuffer, 'K');
    CircularBufferWriteByte(&circularBuffer, 'L');
    CircularBufferWriteByte(&circularBuffer, 'M');
    CircularBufferWriteByte(&circularBuffer, 'N');
    CircularBufferWriteByte(&circularBuffer, 'O');
    CircularBufferWriteByte(&circularBuffer, 'P');
    CircularBufferWriteByte(&circularBuffer, 'Q');
    CircularBufferWriteByte(&circularBuffer, 'R');

    BYTES_EQUAL('K', CircularBufferReadByte(&circularBuffer));

    CircularBufferRewind(&circularBuffer);
    BYTES_EQUAL('J', CircularBufferReadByte(&circularBuffer));
}

TEST(CircularBufferBasic, canWriteMultipleBytes)
{
    uint8_t writeBuffer[5] = {'A', 'B', 'C', 'D', 'E'};

    CircularBufferWriteNBytes(&circularBuffer, writeBuffer, 5);
    BYTES_EQUAL('A', CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('B', CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('C', CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('D', CircularBufferReadByte(&circularBuffer));
    BYTES_EQUAL('E', CircularBufferReadByte(&circularBuffer));

}

TEST(CircularBufferBasic, singleByteWriteFunctionReportsOverwriting){
    uint8_t writeBuffer[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
    CHECK_EQUAL(0, CircularBufferWriteByte(&circularBuffer, writeBuffer[0]));
    CHECK_EQUAL(0, CircularBufferWriteByte(&circularBuffer, writeBuffer[1]));
    CHECK_EQUAL(0, CircularBufferWriteByte(&circularBuffer, writeBuffer[2]));
    CHECK_EQUAL(0, CircularBufferWriteByte(&circularBuffer, writeBuffer[3]));
    CHECK_EQUAL(0, CircularBufferWriteByte(&circularBuffer, writeBuffer[4]));
    CHECK_EQUAL(0, CircularBufferWriteByte(&circularBuffer, writeBuffer[5]));
    CHECK_EQUAL(0, CircularBufferWriteByte(&circularBuffer, writeBuffer[6]));
    CHECK_EQUAL(0, CircularBufferWriteByte(&circularBuffer, writeBuffer[7]));
    CHECK_EQUAL(0, CircularBufferWriteByte(&circularBuffer, writeBuffer[8]));
    CHECK_EQUAL(-1, CircularBufferWriteByte(&circularBuffer, writeBuffer[9]));
}

TEST(CircularBufferBasic, multipleByteWriteFunctionReportsOverwriting){
    uint8_t writeBuffer[10] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
    CHECK_EQUAL(-1,  CircularBufferWriteNBytes(&circularBuffer, writeBuffer, 10));
}

TEST(CircularBufferBasic, multipleByteWriteFunctionReportsOverwritingMoreThanOnceTwo){
    uint8_t writeBuffer[11] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K'};
    CHECK_EQUAL(-2,  CircularBufferWriteNBytes(&circularBuffer, writeBuffer, 11));
}

TEST(CircularBufferBasic, multipleByteWriteFunctionReportsOverwritingMoreThanOnceThree){
    uint8_t writeBuffer[12] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L'};
    CHECK_EQUAL(-3,  CircularBufferWriteNBytes(&circularBuffer, writeBuffer, 12));
}