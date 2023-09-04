#include "bitwriter.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct BitWriter BitWriter;

/* put in bitwriter.c */
#include "io.h"
struct BitWriter {
    Buffer *underlying_stream;
    uint8_t byte;
    uint8_t bit_position;
};

// Allocate a BitWriter object, BitWriter *buf.
// Create Buffer *underlying_stream using write_open().
// buf->underlying_stream = underlying_stream
// Return a pointer to the new BitWriter object, buf.
// If unable to perform any of the prior steps, report an error and end the program.
BitWriter *bit_write_open(const char *filename) {
    BitWriter *b = calloc(1, sizeof(BitWriter));
    b->underlying_stream = write_open(filename);
    if (b->underlying_stream == NULL) {
        //assert error ig?
        printf("the file couldn't be opened ig");
        return NULL;
    }
    return b;
}

//Frees given BitWriter
void bit_write_close(BitWriter **pbuf) {
    //since we are writing bytes in groups of 8 when we close the file we gotta write whatevers left in the buffer
    //in case there are some leftover bytes in it.
    if ((*pbuf)->bit_position > 0) {
        write_uint8((*pbuf)->underlying_stream, (*pbuf)->byte);
    }
    write_close(&((*pbuf)->underlying_stream));
    free(*pbuf);
    *pbuf = NULL;
}

//writes bit to file in BitWriter->underlying_stream
//we do this by sets of 8 bits.
void bit_write_bit(BitWriter *buf, uint8_t x) {
    //if there are(maybe more(?)) than 8 bits in the buffer, we write the 8 bits to the file
    if (buf->bit_position > 7) {
        write_uint8(buf->underlying_stream, buf->byte);
        buf->byte = 0x00;
        buf->bit_position = 0;
    }
    //if x is one, then we shift this value by bit position to put it in the write place and then logical
    //or it with byte.
    if (x & 1) {
        buf->byte |= (x & 1) << buf->bit_position;
    }
    //increment postion
    buf->bit_position++;
}

//exp:
//10101011 i = 0 shift right by 0 and with 00000001
//becomes 00000001 write that
//i=1 shift x by 1 01010101 and with 00000001
//we get 1 etc
void bit_write_uint8(BitWriter *buf, uint8_t x) {
    //for i < 8 so for each byte, shift x right by i and and it with 00000001
    //to get the ith bit
    for (int i = 0; i < 8; i++) {
        bit_write_bit(buf, (x >> i) & 1);
    }
}

void bit_write_uint16(BitWriter *buf, uint16_t x) {
    for (int i = 0; i < 16; i++) {
        bit_write_bit(buf, (x >> i) & 1);
    }
}

void bit_write_uint32(BitWriter *buf, uint32_t x) {
    for (int i = 0; i < 32; i++) {
        bit_write_bit(buf, (x >> i) & 1);
    }
}
